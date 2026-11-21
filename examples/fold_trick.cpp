/// @file fold_trick.cpp
/// Fold trick-art: exact-rational anamorphosis of a 3-D object onto a foldable
/// sheet (C++ port of the Python `chair_flat.py` / `table_flat.py` /
/// `house_flat.py`).
///
/// A viewer V looks at an object standing in a right-angle corner (floor
/// z = 0, wall y = 0, crease = x-axis). Each object point is recorded on
/// whichever plane its sight ray reaches first; the planes are then unfolded
/// into one flat sheet. The plane reached first is decided by the linear form
///
///     sigma(P) = Vz*Py - Vy*Pz      (< 0 wall flap, > 0 floor flap)
///
/// Faces and edges that straddle sigma = 0 are clipped there and each piece is
/// projected onto its own plane. Unfolding maps (x,0,z) -> (x,z) on the wall
/// and (x,y,0) -> (x,-y) on the floor, so v = 0 is the crease.
///
/// Everything is exact: rationals use fun::Frac (Fraction<std::int64_t>) and
/// the projective maps use fun::projective_transform from projgeom.
///
/// Usage:
///     fold_trick [output-dir]
/// Writes chair_flat.svg, table_flat.svg and house_flat.svg.

#include <projgeom/transform.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {

using fun::Frac;
using Pt2 = std::pair<Frac, Frac>;

struct V3 {
    Frac x, y, z;
};

using Face = std::pair<std::string, std::vector<V3>>;
using Edge = std::pair<V3, V3>;
using Piece = std::pair<std::vector<V3>, std::vector<Pt2>>;

struct Scene {
    Frac vx, vy, vz;
    std::vector<Face> faces;
    std::vector<Edge> edges;
    std::string title;
    std::string filename;
};

const Frac SHEET_W{210};
const Frac SHEET_H{297};
const int PX_PER_MM = 4;
const Frac MARGIN{16};

const char* PAPER = "#ffffff";
const char* INK = "#111111";
const char* CREASE = "#d21f1f";

Frac sigma(const Scene& s, const V3& p) { return s.vz * p.y - s.vy * p.z; }

Pt2 project(const Scene& s, const V3& p) {
    if (sigma(s, p) <= Frac{0}) {
        const Frac t = s.vy / (s.vy - p.y);
        return {s.vx + t * (p.x - s.vx), s.vz + t * (p.z - s.vz)};
    }
    const Frac t = s.vz / (s.vz - p.z);
    return {s.vx + t * (p.x - s.vx), -(s.vy + t * (p.y - s.vy))};
}

PgPoint to_pg(const Pt2& q) {
    return PgPoint(fun::rationalize({q.first, q.second, Frac{1}}));
}

Pt2 from_pg(const PgPoint& g) {
    return {Frac(g.coord[0], g.coord[2]), Frac(g.coord[1], g.coord[2])};
}

V3 lerp3(const V3& a, const V3& b, const Frac& t) {
    return {a.x + t * (b.x - a.x), a.y + t * (b.y - a.y), a.z + t * (b.z - a.z)};
}

std::vector<V3> clip_half(const Scene& s, const std::vector<V3>& poly, const Frac& keep) {
    std::vector<V3> out;
    const std::size_t n = poly.size();
    for (std::size_t i = 0; i < n; ++i) {
        const V3& a = poly[i];
        const V3& b = poly[(i + 1) % n];
        const Frac sa = keep * sigma(s, a);
        const Frac sb = keep * sigma(s, b);
        if (sa >= Frac{0}) {
            out.push_back(a);
        }
        if ((sa > Frac{0} && sb < Frac{0}) || (sa < Frac{0} && sb > Frac{0})) {
            out.push_back(lerp3(a, b, sa / (sa - sb)));
        }
    }
    return out;
}

std::vector<Piece> face_pieces(const Scene& s, const std::vector<V3>& poly) {
    bool all_neg = true;
    bool all_pos = true;
    for (const auto& p : poly) {
        const Frac sg = sigma(s, p);
        if (sg > Frac{0}) all_neg = false;
        if (sg < Frac{0}) all_pos = false;
    }
    if ((all_neg || all_pos) && poly.size() == 4) {
        const std::array<PgPoint, 4> src{PgPoint({0, 0, 1}), PgPoint({1, 0, 1}), PgPoint({1, 1, 1}), PgPoint({0, 1, 1})};
        const std::array<PgPoint, 4> dst{to_pg(project(s, poly[0])), to_pg(project(s, poly[1])), to_pg(project(s, poly[2])), to_pg(project(s, poly[3]))};
        try {
            const auto tr = fun::projective_transform(src, dst);
            std::vector<Pt2> out;
            for (const auto& c : src) {
                out.push_back(from_pg(tr.apply_point(c)));
            }
            return {Piece{poly, out}};
        } catch (...) {
            std::vector<Pt2> out;
            for (const auto& p : poly) {
                out.push_back(project(s, p));
            }
            return {Piece{poly, out}};
        }
    }
    std::vector<Piece> pieces;
    for (const auto& keep : {Frac{1}, Frac{-1}}) {
        auto piece = clip_half(s, poly, keep);
        if (piece.size() >= 3) {
            std::vector<Pt2> out;
            for (const auto& p : piece) {
                out.push_back(project(s, p));
            }
            pieces.push_back(Piece{piece, out});
        }
    }
    return pieces;
}

std::vector<Edge> edge_segments(const Scene& s, const V3& a, const V3& b) {
    const Frac sa = sigma(s, a);
    const Frac sb = sigma(s, b);
    if (sa == Frac{0} || sb == Frac{0} || (sa > Frac{0}) == (sb > Frac{0})) {
        return {Edge{a, b}};
    }
    const V3 mid = lerp3(a, b, sa / (sa - sb));
    return {Edge{a, mid}, Edge{mid, b}};
}

Frac depth2(const Scene& s, const V3& c) {
    const Frac dx = c.x - s.vx;
    const Frac dy = c.y - s.vy;
    const Frac dz = c.z - s.vz;
    return dx * dx + dy * dy + dz * dz;
}

V3 centroid(const std::vector<V3>& poly) {
    const Frac n = Frac(static_cast<std::int64_t>(poly.size()));
    Frac x{0}, y{0}, z{0};
    for (const auto& p : poly) {
        x = x + p.x;
        y = y + p.y;
        z = z + p.z;
    }
    return {x / n, y / n, z / n};
}

std::int64_t round_frac(const Frac& value) {
    const std::int64_t num = value.num();
    const std::int64_t den = value.den();
    return (num >= 0) ? (num + den / 2) / den : -((-num + den / 2) / den);
}

struct Fit {
    Frac scale, off_u, off_v;
};

Fit make_fit(const std::vector<Pt2>& pts) {
    Frac umin = pts[0].first, umax = pts[0].first, vmin = pts[0].second, vmax = pts[0].second;
    for (const auto& p : pts) {
        umin = std::min(umin, p.first);
        umax = std::max(umax, p.first);
        vmin = std::min(vmin, p.second);
        vmax = std::max(vmax, p.second);
    }
    const Frac span_u = std::max(umax - umin, Frac{1});
    const Frac span_v = std::max(vmax - vmin, Frac{1});
    const Frac scale = std::min((SHEET_W - 2 * MARGIN) / span_u, (SHEET_H - 2 * MARGIN) / span_v);
    const Frac off_u = MARGIN + (SHEET_W - 2 * MARGIN - span_u * scale) * Frac(1, 2) - umin * scale;
    const Frac off_v = MARGIN + (SHEET_H - 2 * MARGIN - span_v * scale) * Frac(1, 2) - vmin * scale;
    return {scale, off_u, off_v};
}

std::pair<std::int64_t, std::int64_t> to_px(const Pt2& p, const Fit& fit) {
    const Frac u = p.first * fit.scale + fit.off_u;
    const Frac v = p.second * fit.scale + fit.off_v;
    return {round_frac(u * Frac(PX_PER_MM)), round_frac((SHEET_H - v) * Frac(PX_PER_MM))};
}

struct DrawPiece {
    Frac depth;
    const std::string* color;
    const std::vector<Pt2>* poly;
};

std::string render(const Scene& scene) {
    std::vector<Piece> pieces;
    std::vector<std::string> piece_color;
    for (const auto& f : scene.faces) {
        for (auto& piece : face_pieces(scene, f.second)) {
            piece_color.push_back(f.first);
            pieces.push_back(std::move(piece));
        }
    }

    std::vector<DrawPiece> draw(pieces.size());
    std::vector<std::pair<Pt2, Pt2>> edge_list;
    std::vector<Pt2> all_pts;
    for (std::size_t i = 0; i < pieces.size(); ++i) {
        draw[i] = DrawPiece{depth2(scene, centroid(pieces[i].first)), &piece_color[i], &pieces[i].second};
        for (const auto& q : pieces[i].second) {
            all_pts.push_back(q);
        }
    }
    for (const auto& e : scene.edges) {
        for (const auto& seg : edge_segments(scene, e.first, e.second)) {
            const Pt2 a = project(scene, seg.first);
            const Pt2 b = project(scene, seg.second);
            edge_list.push_back({a, b});
            all_pts.push_back(a);
            all_pts.push_back(b);
        }
    }

    std::stable_sort(draw.begin(), draw.end(), [](const DrawPiece& a, const DrawPiece& b) { return a.depth > b.depth; });

    const Fit fit = make_fit(all_pts);

    Frac umin = all_pts[0].first, umax = all_pts[0].first;
    for (const auto& p : all_pts) {
        umin = std::min(umin, p.first);
        umax = std::max(umax, p.first);
    }

    std::ostringstream doc;
    doc << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << SHEET_W.num() << "mm\" height=\"" << SHEET_H.num() << "mm\" viewBox=\"0 0 "
        << round_frac(SHEET_W * Frac(PX_PER_MM)) << " " << round_frac(SHEET_H * Frac(PX_PER_MM)) << "\">\n";
    doc << "<title>" << scene.title << "</title>\n";
    doc << "<rect width=\"100%\" height=\"100%\" fill=\"" << PAPER << "\"/>\n";
    for (const auto& d : draw) {
        doc << "<polygon points=\"";
        for (std::size_t i = 0; i < d.poly->size(); ++i) {
            const auto px = to_px((*d.poly)[i], fit);
            if (i != 0) doc << " ";
            doc << px.first << "," << px.second;
        }
        doc << "\" fill=\"" << *d.color << "\" stroke=\"none\"/>\n";
    }
    for (const auto& e : edge_list) {
        const auto a = to_px(e.first, fit);
        const auto b = to_px(e.second, fit);
        doc << "<line x1=\"" << a.first << "\" y1=\"" << a.second << "\" x2=\"" << b.first << "\" y2=\"" << b.second
            << "\" stroke=\"" << INK << "\" stroke-width=\"2\"/>\n";
    }
    const auto cl = to_px({umin - Frac{14}, Frac{0}}, fit);
    const auto ch = to_px({umax + Frac{14}, Frac{0}}, fit);
    doc << "<line x1=\"" << cl.first << "\" y1=\"" << cl.second << "\" x2=\"" << ch.first << "\" y2=\"" << ch.second
        << "\" stroke=\"" << CREASE << "\" stroke-width=\"2\" stroke-dasharray=\"12 8\"/>\n";
    doc << "<text x=\"" << ch.first << "\" y=\"" << (ch.second - 8) << "\" font-size=\"20\" fill=\"" << CREASE
        << "\" text-anchor=\"end\">fold here (crease)</text>\n";
    doc << "</svg>\n";
    return doc.str();
}

Scene make_chair() {
    const Frac X0{-30}, X1{30};
    const Frac Z{55}, T{10}, Y0{64}, Y1{104}, TOP{103}, GB{83}, ST{5}, LG{3};
    const Frac z0{0};
    Scene s;
    s.vx = Frac{-120};
    s.vy = Frac{280};
    s.vz = Frac{220};
    s.title = "Fold-a-chair trick art";
    s.filename = "chair_flat.svg";
    s.faces = {
        {"#3aa93a", {{X0 + ST, Y0, GB}, {X1 - ST, Y0, GB}, {X1 - ST, Y0, TOP - ST}, {X0 + ST, Y0, TOP - ST}}},
        {"#e2701f", {{X0, Y0, Z}, {X1, Y0, Z}, {X1, Y1, Z}, {X0, Y1, Z}}},
        {"#b4d9ea", {{X0, Y1, Z - T}, {X1, Y1, Z - T}, {X1, Y1, Z}, {X0, Y1, Z}}},
        {"#1f6fd0", {{X0, Y0, Z - T}, {X0, Y1, Z - T}, {X0, Y1, Z}, {X0, Y0, Z}}},
    };
    s.edges = {
        {{X0, Y0, Z}, {X0, Y0, TOP}}, {{X1, Y0, Z}, {X1, Y0, TOP}}, {{X0, Y0, TOP}, {X1, Y0, TOP}},
        {{X0, Y0, GB}, {X1, Y0, GB}}, {{X0, Y0, Z}, {X1, Y0, Z}}, {{X1, Y0, Z}, {X1, Y1, Z}},
        {{X1, Y1, Z}, {X0, Y1, Z}}, {{X0, Y1, Z}, {X0, Y0, Z}}, {{X0, Y1, Z - T}, {X1, Y1, Z - T}},
        {{X0, Y0, Z - T}, {X0, Y1, Z - T}}, {{X0, Y1, Z - T}, {X0, Y1, Z}}, {{X1, Y1, Z - T}, {X1, Y1, Z}},
    };
    for (const Frac lx : {X0 + LG * Frac(1, 2), X1 - LG * Frac(1, 2)}) {
        for (const Frac ly : {Y0 + LG * Frac(1, 2), Y1 - LG * Frac(1, 2)}) {
            s.edges.push_back({{lx, ly, Z - T}, {lx, ly, z0}});
            s.edges.push_back({{lx - LG * Frac(1, 2), ly, z0}, {lx + LG * Frac(1, 2), ly, z0}});
        }
    }
    return s;
}

Scene make_table() {
    const Frac X0{-50}, X1{50};
    const Frac TOP{74}, T{8}, Y0{30}, Y1{90}, LG{7};
    const Frac z0{0};
    Scene s;
    s.vx = Frac{-130};
    s.vy = Frac{280};
    s.vz = Frac{230};
    s.title = "Fold-a-table trick art";
    s.filename = "table_flat.svg";
    s.faces = {
        {"#cf7b32", {{X0, Y0, TOP}, {X1, Y0, TOP}, {X1, Y1, TOP}, {X0, Y1, TOP}}},
        {"#e0a96d", {{X0, Y1, TOP - T}, {X1, Y1, TOP - T}, {X1, Y1, TOP}, {X0, Y1, TOP}}},
        {"#9c5a22", {{X0, Y0, TOP - T}, {X0, Y1, TOP - T}, {X0, Y1, TOP}, {X0, Y0, TOP}}},
    };
    s.edges = {
        {{X0, Y0, TOP}, {X1, Y0, TOP}}, {{X1, Y0, TOP}, {X1, Y1, TOP}}, {{X1, Y1, TOP}, {X0, Y1, TOP}},
        {{X0, Y1, TOP}, {X0, Y0, TOP}}, {{X0, Y1, TOP - T}, {X1, Y1, TOP - T}}, {{X0, Y0, TOP - T}, {X0, Y1, TOP - T}},
        {{X0, Y1, TOP - T}, {X0, Y1, TOP}}, {{X1, Y1, TOP - T}, {X1, Y1, TOP}},
    };
    for (const Frac lx : {X0 + LG * Frac(1, 2), X1 - LG * Frac(1, 2)}) {
        for (const Frac ly : {Y0 + LG * Frac(1, 2), Y1 - LG * Frac(1, 2)}) {
            s.edges.push_back({{lx, ly, TOP - T}, {lx, ly, z0}});
            s.edges.push_back({{lx - LG * Frac(1, 2), ly, z0}, {lx + LG * Frac(1, 2), ly, z0}});
        }
    }
    return s;
}

Scene make_house() {
    const Frac X0{-40}, X1{40};
    const Frac Y0{38}, Y1{100}, MID{(Y0 + Y1) * Frac(1, 2)};
    const Frac WZ{80}, RZ{116}, DW{11}, DH{42};
    const Frac WX0{13}, WX1{31}, WZ0{52}, WZ1{66};
    const Frac EPS{1, 2};
    const Frac z0{0};
    Scene s;
    s.vx = Frac{-150};
    s.vy = Frac{280};
    s.vz = Frac{240};
    s.title = "Fold-a-house trick art";
    s.filename = "house_flat.svg";
    s.faces = {
        {"#ece5d6", {{X0, Y1, z0}, {X1, Y1, z0}, {X1, Y1, WZ}, {X0, Y1, WZ}}},
        {"#cec4ab", {{X0, Y0, z0}, {X0, Y1, z0}, {X0, Y1, WZ}, {X0, Y0, WZ}}},
        {"#cec4ab", {{X0, Y0, WZ}, {X0, Y1, WZ}, {X0, MID, RZ}}},
        {"#b5523a", {{X0, Y1, WZ}, {X1, Y1, WZ}, {X1, MID, RZ}, {X0, MID, RZ}}},
        {"#7a4a2b", {{-DW, Y1 + EPS, z0}, {DW, Y1 + EPS, z0}, {DW, Y1 + EPS, DH}, {-DW, Y1 + EPS, DH}}},
        {"#8fd0e8", {{WX0, Y1 + EPS, WZ0}, {WX1, Y1 + EPS, WZ0}, {WX1, Y1 + EPS, WZ1}, {WX0, Y1 + EPS, WZ1}}},
    };
    s.edges = {
        {{X0, Y0, z0}, {X1, Y0, z0}}, {{X1, Y0, z0}, {X1, Y1, z0}}, {{X1, Y1, z0}, {X0, Y1, z0}}, {{X0, Y1, z0}, {X0, Y0, z0}},
        {{X0, Y1, z0}, {X0, Y1, WZ}}, {{X1, Y1, z0}, {X1, Y1, WZ}}, {{X0, Y0, z0}, {X0, Y0, WZ}}, {{X0, Y0, WZ}, {X0, Y1, WZ}},
        {{X0, Y1, WZ}, {X1, Y1, WZ}}, {{X0, Y0, WZ}, {X0, MID, RZ}}, {{X0, MID, RZ}, {X0, Y1, WZ}},
        {{X0, MID, RZ}, {X1, MID, RZ}}, {{X1, MID, RZ}, {X1, Y1, WZ}},
        {{-DW, Y1, z0}, {DW, Y1, z0}}, {{-DW, Y1, z0}, {-DW, Y1, DH}}, {{DW, Y1, z0}, {DW, Y1, DH}}, {{-DW, Y1, DH}, {DW, Y1, DH}},
        {{WX0, Y1, WZ0}, {WX1, Y1, WZ0}}, {{WX0, Y1, WZ1}, {WX1, Y1, WZ1}}, {{WX0, Y1, WZ0}, {WX0, Y1, WZ1}}, {{WX1, Y1, WZ0}, {WX1, Y1, WZ1}},
    };
    return s;
}

bool write_file(const std::string& path, const std::string& content) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        return false;
    }
    out << content;
    return static_cast<bool>(out);
}

}  // namespace

int main(int argc, char** argv) {
    const std::string dir = (argc > 1) ? std::string(argv[1]) : std::string(".");
    const std::string sep = dir.empty() ? "" : "/";

    for (const Scene& scene : {make_chair(), make_table(), make_house()}) {
        const std::string path = dir + sep + scene.filename;
        if (!write_file(path, render(scene))) {
            std::cerr << "failed to write " << path << "\n";
            return EXIT_FAILURE;
        }
        std::cout << "wrote " << path << "\n";
    }
    return EXIT_SUCCESS;
}
