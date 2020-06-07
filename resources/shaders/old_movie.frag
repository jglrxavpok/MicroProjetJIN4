// Modified version of: https://www.shadertoy.com/view/4dKfDm

uniform sampler2D iChannel0;
uniform float iTime;

const vec2 iResolution = vec2(1600,900);

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}
vec2 hash22(vec2 p)
{
    p = vec2(dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)));
    return -1.0 + 2.0 * fract(sin(p)*43758.5453123);
}
float nos(vec2 p)
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;
    vec2 i = floor(p + (p.x + p.y) * K1);
    vec2 a = p - (i - (i.x + i.y) * K2);
    vec2 o = (a.x < a.y) ? vec2(0.0, 1.0) : vec2(1.0, 0.0);
    vec2 b = a - o + K2;
    vec2 c = a - 1.0 + 2.0 * K2;
    vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
    vec3 n = h * h * h * h * vec3(dot(a, hash22(i)), dot(b, hash22(i + o)), dot(c, hash22(i + 1.0)));
    return dot(vec3(70.0, 70.0, 70.0), n);
}
#define film mat3(0.393, 0.769, 0.189, 0.349, 0.686, 0.168, 0.272, 0.534, 0.131)
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 pos = fragCoord/iResolution;
    //screen dither x：
    vec2 nos_pos = pos - 0.5 + vec2(min(fract(iTime) - 0.02, 0.0), 0.0);
    vec2 abs_n_p = vec2(abs(nos_pos));
    //screen dither y：
    vec2 P = pos + vec2(0.0, max(min(40.0 * fract(iTime) - 38.0, -40.0 * fract(iTime) + 40.0), 0.0))/vec2(20.0);
    vec3 Col = texture(iChannel0, P).rgb;

    //screen scratch：
    float coord = rand(vec2(floor(4.0 * iTime), pos.x));
    if(abs(pos.x - coord) <= 0.001)
    Col += 0.5 + 0.5 * nos(vec2(20.0, 2.0) * pos);
    //light spots：
    if(fract(0.1 * iTime) > 0.9)
    {
        Col += step(nos(6.0 * (pos + vec2(floor(4.0 * fract(iTime)), 0.0))), -0.8);
        if(fract(0.1 * iTime) > 0.98)
        Col *= 1.0 + 0.25 * abs(sin(20.0 * fract(2.0 * iTime)));
    }
    fragColor = vec4(clamp(Col, 0.0, 1.0),1.0);
}

void main()
{
    vec4 finalColor = vec4(1.0);
    mainImage(finalColor, gl_TexCoord[0].xy*iResolution);
    gl_FragColor = finalColor;
}
