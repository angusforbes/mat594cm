varying vec3 normal; //the interpolated fragment normal in eye coordinates
varying vec4 pos; //the interpolated fragment position in eye coordinates

void main() {
  vec4 color = gl_FrontMaterial.diffuse;
  vec4 matspec = gl_FrontMaterial.specular;
  float shininess = gl_FrontMaterial.shininess;
  vec4 lightspec = gl_LightSource[0].specular;
  vec4 lpos = gl_LightSource[0].position; //light position in eye coordinates
  vec4 s = -normalize(pos-lpos); //vector from fragment to light

  vec3 light = s.xyz;
  vec3 n = normalize(normal);
  vec3 r = normalize(-reflect(light, n)); //vector from fragment pointing halfway between light and camera
  vec3 v = normalize(-pos.xyz); //vector pointing to camera
   
  //calculate SPECULAR lighting  
  vec4 specular;
  if (shininess != 0.0) {
    specular = lightspec * matspec * pow(max(0.0, dot(r, v)), shininess);
  } else {
    specular = vec4(0.0, 0.0, 0.0, 0.0);
  }

  //calculate DIFFUSE lighting 
  vec4 diffuse  = color * max(0.0, dot(n, s.xyz)) * gl_LightSource[0].diffuse;
 
  gl_FragColor = diffuse + specular;
}

