#version 460 core

// Vertex Shader Inputs
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;
uniform float Time;

float density = 0.5;
float opacityScanline = 0.3;
float opacityNoise = 0.2;
float flickering = 0.03;


out vec4 FinalColor;

float random (vec2 st) 
{
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}


float blend(const in float x, const in float y) 
{
	return (x < 0.5) ? (2.0 * x * y) : (1.0 - 2.0 * (1.0 - x) * (1.0 - y));
}


vec3 blend(const in vec3 x, const in vec3 y, const in float opacity) 
{
	vec3 z = vec3(blend(x.r, y.r), blend(x.g, y.g), blend(x.b, y.b));
	return z * opacity + x * (1.0 - opacity);
}


void main()
{
    vec2 uv = FragTexCoords;
    vec3 col = texture(ImageTexture, uv).rgb;
    
    float count = 800 * density;
    vec2 sl = vec2(sin(uv.y * count), cos(uv.y * count));
	vec3 scanlines = vec3(sl.x, sl.y, sl.x);

    col += col * scanlines * opacityScanline;
    col += col * vec3(random(uv*Time)) * opacityNoise;
    col += col * sin(110.0*Time) * flickering;


    FinalColor = vec4(col, 1.0);
}