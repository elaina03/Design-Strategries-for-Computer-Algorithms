// 2-dim linear programming
#include <bits/stdc++.h>
#include <limits>
using namespace std;

#define INF numeric_limits<double>::infinity()
// typedef point(x,y)
using point = pair<double, double>;
// global variables
// const auto INF = numeric_limits<double>::infinity();
const double eps = 1.e-10;
double RightBound = 1e20;
double LeftBound = -1e20;
double y_min = -INF;
double y_max = INF;
double answer = numeric_limits<double>::quiet_NaN();

struct line {
    // ax + by <= c
    int a,b,c;
    double slope;
    line(int a, int b, int c): a(a), b(b), c(c) {
        slope = -a * 1. / b;
    }
    bool pass_through(const point p) const {
        return abs(a * p.first + b * p.second - c) <= eps;
    }
};

// record line index and intersection point
struct intersection {
    int line1_idx, line2_idx;
    point p;
    bool operator < (const intersection& that) const {
        return p < that.p;
    }
};
// compute intersection point
// input: two line segments, output: intersection point(x,y)
point Intersect(const line& line1, const line& line2) {
    int det = line1.a * line2.b - line1.b * line2.a;
    if (det == 0) {
        return point(INF, INF);
    }
    double x = (line1.c * line2.b - line1.b * line2.c) * 1. / det;
    double y = (line1.a * line2.c - line1.c * line2.a) * 1. / det;
    return point(x, y);
}

// using x_median, compute alpha and beta
pair<double, double> ComputeAlphaBeta( double x_median, const set<int>& I_negative_idx, const set<int>& I_positive_idx);
// compute  pairs of constraints(bounding lines)
// input: line segment index set and intersection points vector
// add intersection point into this vector
void PairsConstraints( set<int>& line_idx, vector<intersection>& points);
// solution
void Prune_and_Search(int n);
// global variables
vector<line> lines; // Problem Set
// I_negative: I- for b < 0, index set
set<int>  I_negative;
// I_positive: I+ for b > 0, index set
set<int>  I_positive;

int main(int argc, char** argv)
{
    // read input
    int num; // number of constraints

    cin >> num;
    // capacity change, enough to contain num elements
    lines.reserve(num);
    for (int i = 0; i < num; i++) {
        int a, b, c;
        // ax + by <= c
        cin >> a >> b >> c;
        // Inserts a new element at the end of the vector, constructed in place
        lines.emplace_back(a, b, c);
        if( b < 0){ // I-
            if (a == 0) {
                y_min = max(y_min, c * 1. / b);
            }
            I_negative.insert(i);
        }else if (b > 0){ // I+
            if (a == 0) {
                y_max = min(y_max, c * 1. / b);
            }
            I_positive.insert(i);
        }else{ // update LeftBound, RightBound
            double Bound = c * 1. / a;
            if (a > 0) {
                // x <= c / a
                RightBound = min(RightBound, Bound);
            } else {
                // x >= c / a
                LeftBound = max(LeftBound, Bound);
            }
        } // end if else
    } // end for read input

    // ifstream infile(argv[1]);
    // if( !infile){
    //     cerr << "Cannot open the File: "<< argv[1] << endl;
    //     // return false;
    // }

    // // read input
    // infile >> num;
    // // capacity change, enough to contain num elements
    // lines.reserve(num);
    // for (int i = 0; i < num; i++) {
    //     int a, b, c;
    //     // ax + by <= c
    //     infile >> a >> b >> c;
    //     // Inserts a new element at the end of the vector, constructed in place
    //     lines.emplace_back(a, b, c);
    //     if( b < 0){ // I-
    //         if (a == 0) {
    //             y_min = max(y_min, c * 1. / b);
    //         }
    //         I_negative.insert(i);
    //     }else if (b > 0){ // I+
    //         if (a == 0) {
    //             y_max = min(y_max, c * 1. / b);
    //         }
    //         I_positive.insert(i);
    //     }else{ // update LeftBound, RightBound
    //         double Bound = c * 1. / a;
    //         if (a > 0) {
    //             // x <= c / a
    //             RightBound = min(RightBound, Bound);
    //         } else {
    //             // x >= c / a
    //             LeftBound = max(LeftBound, Bound);
    //         }
    //     } // end if else
    // } // end for read input

    Prune_and_Search(num);

    return 0;
} // end main

pair<double, double> ComputeAlphaBeta( double x_median, const set<int>& I_negative_idx, const set<int>& I_positive_idx)
{
    double alpha = -INF;
    double beta = INF;

    for(auto idx: I_negative_idx){
        auto& line = lines[idx];
        alpha = max( alpha, (line.c- line.a*x_median)/line.b );
    }

    for(auto idx: I_positive_idx){
        auto& line = lines[idx];
        beta = min( beta, (line.c- line.a*x_median)/line.b );
    }

    return make_pair( alpha, beta);
} // end ComputeAlphaBeta

void PairsConstraints( set<int>& line_idx, vector<intersection>& points){
    set<int> removed_lines_idx;
    for (auto it_x = line_idx.begin(); it_x != line_idx.end(); ++it_x) {
        auto it_y = next(it_x);
        // reach the end of index set
        if (it_y == line_idx.end()) break;
        // pair of constraint
        auto& x = lines[*it_x];
        auto& y = lines[*it_y];
        point p = Intersect(x, y); // intersection point

        if (p.first == INF) { // line parallel
            // constant of two lines
            double cx = x.c * 1. / x.b;
            double cy = y.c * 1. / y.b;
            int removing_idx;
            // when in I+, prune the larger constant line
            if (x.b > 0) {
                removing_idx = (cx >= cy) ? *it_x : *it_y;
            } else { // in I-, prune the smaller constant line
                removing_idx = (cx <= cy) ? *it_x : *it_y;
            }
            removed_lines_idx.insert(removing_idx);
        } else { // found intersection point

            bool beyond_left = p.first < (LeftBound - eps);
            bool beyond_right = p.first > (RightBound + eps);

            if (beyond_left || beyond_right) {
                int removing_idx;
                if (beyond_left == (x.b < 0)) {
                    // beyond LeftBoundary and in I-, discard smaller slope line
                    // beyong RightBoundary and in I+, discard smaller slope line
                    removing_idx = x.slope < y.slope ? *it_x : *it_y;
                } else {
                    // beyond RightBoundary and in I-, discard larger slope line
                    // beyong LeftBoundary and in I+, discard larger slope line
                    removing_idx = x.slope > y.slope ? *it_x : *it_y;
                }
                removed_lines_idx.insert(removing_idx);
            } else {
                points.emplace_back((intersection){*it_x, *it_y, p});
            } // end else
        } // end else
        ++it_x;
    } // end for

    // remove parallel lines
    for (auto it = removed_lines_idx.begin(); it != removed_lines_idx.end(); ++it) {
        line_idx.erase(*it);
    }
} // end PairsConstraints

void Prune_and_Search(int n)
{
    enum { UNKNOWN, LEFT_SIDE, RIGHT_SIDE, SOLVED, INFEASIBLE} status;

    while (1) {
        // vector contain intersection points
        vector<intersection> points;
        status = UNKNOWN;

        if (!(LeftBound <= RightBound)) {
            status = INFEASIBLE;
            break;
        }

        if (I_negative.size() <= 1 && I_positive.size() <= 1) {
            break;
        }
        // n constraints, find (n/2) intersection points
        PairsConstraints(I_negative, points);
        PairsConstraints(I_positive, points);

        if (points.size() == 0) {
            // no intersection point
            continue;
        }

        // find x_median of points
        auto it_median = points.begin() + (points.size() - 1) / 2;
        // Rearranges the elements, the element at the nth position is
        // the element that would be in that position in a sorted sequence
        nth_element(points.begin(), it_median, points.end());

        // x_m: median value of x
        double x_m = it_median->p.first;
        // y_range: y value under x_median
        auto y_range = ComputeAlphaBeta(x_m, I_negative, I_positive);
        double alpha = y_range.first;
        double beta = y_range.second;

        double s_min = INF, s_max = -INF;
        // compute s_min, s_max under alpha
        for (auto i: I_negative) {
            auto& line = lines[i];
            if (line.pass_through(point(x_m, alpha))) {
                s_min = min(s_min, line.slope);
                s_max = max(s_max, line.slope);
            }
        }

        double t_min = INF, t_max = -INF;
        // compute t_min, t_max under beta
        for (auto i: I_positive) {
            auto& line = lines[i];
            if (line.pass_through(point(x_m, beta))) {
                t_min = min(t_min, line.slope);
                t_max = max(t_max, line.slope);
            }
        }

        if (alpha <= beta) { // in feasible region
            if (s_max * s_min <= 0) { // case 3
                status = SOLVED;
                answer = alpha;
            } else if (s_max < 0) { // case1
                status = RIGHT_SIDE;
            } else {  // s_min > 0, case 2
                status = LEFT_SIDE;
            }
        } else { // not in feasible region
            if (s_max < t_min) { // case 4
                // x* > x_m; x* > intersect(argmax(s), argmin(t))
                status = RIGHT_SIDE;
            } else if (s_min > t_max) { // case 5
                // x* < x_m; x* < intersect(argmin(s), argmax(t))
                status = LEFT_SIDE;
            } else { // case 6
                // no feasible region
                status = INFEASIBLE;
            }
        } // end if else 6 cases

        if (status == RIGHT_SIDE) {
            // update LeftBound
            LeftBound = x_m;
            for (auto& pt: points) {
                if (pt.p.first > x_m) {
                    continue;
                }
                // remove smaller slope in I-, larger slope in I+
                if (lines[pt.line1_idx].slope < lines[pt.line2_idx].slope) {
                    if (!I_negative.erase(pt.line1_idx)) {
                        I_positive.erase(pt.line2_idx);
                    }
                } else {
                    if (!I_negative.erase(pt.line2_idx)) {
                        I_positive.erase(pt.line1_idx);
                    }
                }
            }
        } else if (status == LEFT_SIDE) {
            // update RightBound
            RightBound = x_m;
            for (auto& pt: points) {
                if (pt.p.first < x_m) {
                    continue;
                }
                // remove larger slope in I-, smaller slope in I+
                if (lines[pt.line1_idx].slope < lines[pt.line2_idx].slope) {
                    if (!I_negative.erase(pt.line2_idx)) {
                        I_positive.erase(pt.line1_idx);
                    }
                } else {
                    if (!I_negative.erase(pt.line1_idx)) {
                        I_positive.erase(pt.line2_idx);
                    }
                }
            }
        } else {
            break;
        } // end if else status condition
    } // end while

    if (status != SOLVED && status != INFEASIBLE) { // solution is not yet determined

        // compute y range under x coordinate boundary
        auto yran_l = ComputeAlphaBeta(LeftBound, I_negative, I_positive);
        auto yran_r = ComputeAlphaBeta(RightBound, I_negative, I_positive);

        double y_min_boundary = INF;
        // use the smaller value of two borders
        if (yran_l.first <= yran_l.second) { // in left feasible region
            y_min_boundary = min(y_min_boundary, yran_l.first);
        }
        if (yran_r.first <= yran_r.second) { // in right feasible region
            y_min_boundary = min(y_min_boundary, yran_r.first);
        }

        if (!(y_min <= y_max)) {
            status = INFEASIBLE;
        } else if (yran_l.first > yran_l.second &&
                   yran_r.first > yran_r.second) {
            status = INFEASIBLE;
        } else if (I_negative.size() + I_positive.size() <= 1) { // at most one line
            status = SOLVED;
            answer = max(y_min_boundary, y_min);
        } else if (I_negative.size() == 1 && I_positive.size() == 1) {
            line& lnn = lines[*I_negative.begin()];
            line& lnp = lines[*I_positive.begin()];

            point p = Intersect(lnn, lnp);

            if (p.first == INF && -lnn.c > lnp.c) {
                // two parallel lines
                status = INFEASIBLE;
            } else if (p.first >= (LeftBound - eps) && p.first <= (RightBound + eps) ) {
                if (p.second >= (y_min - eps) && p.second <= (y_max + eps) ) {
                    // point p in feasible region
                    answer = min(y_min_boundary, p.second);
                    status = SOLVED;
                } else {
                    status = INFEASIBLE;
                }
            } else {
                status = SOLVED;
                answer = max(y_min_boundary, y_min);
            }
        } else {
            // should be unreachable
            abort();
        }
    } // end if solution not yet determined

    if (status == SOLVED) {
        if (answer < -1e10) {
            cout << "-INF";
        } else {
            // printf("%.6f\n", answer);
            cout << int(round(answer));
            // printf("%d\n", int(round(answer)));
        }
    } else if (status == INFEASIBLE) {
        cout << "NA";
    } // end if else output solution

} // end Prune_and_Search
