uniform sampler2D blurTex;
uniform sampler2D tex1;
uniform sampler2D tex2;

varying float opacity;

void main()
{
  vec4 gauss = texture2D( blurTex, gl_TexCoord[0].st);
  vec4 base = texture2D( tex1, gl_TexCoord[1].st);
  vec4 blend = texture2D( tex2, gl_TexCoord[2].st);

  vec4 mixed = mix(base, blend, opacity);
  gl_FragColor = vec4(mixed.rgb, gauss.r);
}

