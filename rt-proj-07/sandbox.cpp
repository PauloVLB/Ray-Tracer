#include <bits/stdc++.h>
# include <glm/ext/vector_float2.hpp>
# include <glm/ext/vector_float3.hpp>
# include <glm/ext/vector_int2.hpp>
# include <glm/ext/vector_int3.hpp>

using namespace std;

int main() {
    glm::vec3 v;
    float a = 2.2;
    float b = 3.3;
    float c = 4.2;
    array<float, 3> x{a, b ,c};
    v = glm::vec3{a, b};
    cout << v[0] << " " << v[1] << " " << v[2] << endl;
    //vector<glm::vec3> vec;

    //vec.push_back({a,b,c});
    // vec.push_back(glm::vec3{a,b,c});

    //for(auto x : vec) {
    //    cout << x.s << " " << x.t << " " << x.p << endl;
    //}
}