uniform sampler2D tex1;
uniform float threshold;
uniform int kA; //kernel size
uniform int kB; //kernel size

/*
   void main(void) {
   vec3 luminanceVector = vec3(0.2125, 0.7154, 0.0721);
   vec4 sample = texture2D(tex1, gl_TexCoord[0].st);

   float luminance = dot(luminanceVector, sample.rgb);
   luminance = max(0.0, luminance - threshold);
   sample.rgb *= sign(luminance);
   sample.a = 1.0;

   gl_FragColor = sample;
   }
 */

void main() {
  float t = threshold;
  vec4 sum = vec4(0);
  vec2 texcoord = vec2(gl_TexCoord[0]);
  int j;
  int i;

  for( i= -kA ;i < kA; i++) {
    for (j = -kB; j < kB; j++) {
      sum += texture2D(tex1, texcoord + vec2(j, i)*0.004) * 0.25;
    }
  }
  if (texture2D(tex1, texcoord).r < 0.3) {
    gl_FragColor = sum*sum*0.012 + texture2D(tex1, texcoord);
  } else {
    if (texture2D(tex1, texcoord).r < 0.5) {
      gl_FragColor = sum*sum*0.009 + texture2D(tex1, texcoord);
    } else {
      gl_FragColor = sum*sum*0.0075 + texture2D(tex1, texcoord);
    }
  }
}

