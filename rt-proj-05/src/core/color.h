#ifndef COLOR_H
#define COLOR_H

#include <vector>
using std::vector;

using real_type = float;
using byte = unsigned char;

namespace rt3 {

class Color {
    public:
        real_type r = 0;
        real_type g = 0;
        real_type b = 0;
        real_type alpha = 1;

        Color() {}
        Color(const vector<real_type> &v) {
            r = v[0];
            g = v[1];
            b = v[2];
            alpha = 1;
        }
        Color(real_type mr, real_type mg, real_type mb) : r(mr), g(mg), b(mb) {}

        Color operator * (const real_type t) const;
        Color operator * (const Color c) const;
        Color operator + (const Color c) const;
        real_type& operator [] (int index) {
            if(index == 0) return r;
            if(index == 1) return g;
            if(index == 2) return b;
            return alpha;
        }

        static inline Color real_color(const vector<real_type> &v) {
            return Color(v);
        }

        static inline Color int_color(vector<real_type> &v) {
            for(int i = 0; i < 3; i++) v[i] /= 255.0;
            return real_color(v);
        }

        
        static inline Color lerp_color( const Color &A, const Color &B, float t ) {
            return A*(1-t) + B*t;
        }
};

}

#endif 