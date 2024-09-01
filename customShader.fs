#version 330 core

out vec4 FragColor;

in vec2 Uv;
uniform sampler2D uTexture;
uniform mat3 uKernel;

uniform float uOffset;  


void main(){
    vec2 offsets[] = {vec2(-uOffset,uOffset),
                        vec2(0.0f,   uOffset),
                        vec2(uOffset, uOffset),
                        vec2(-uOffset,0.0f),
                        vec2(0.0f,   0.0f),
                        vec2(uOffset, 0.0f),
                        vec2(-uOffset,-uOffset),
                        vec2(0.0f,   -uOffset),
                        vec2(uOffset, -uOffset)
    };

    FragColor = vec4(0,0,0,1.0f);
    for(int i = 0; i < 9; ++i)
    {
        FragColor += texture(uTexture,Uv.st + offsets[i]) * uKernel[i%3][i/3];
    }
    
    //FragColor = texture(uTexture, Uv);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average,average,average, 1.0);
}