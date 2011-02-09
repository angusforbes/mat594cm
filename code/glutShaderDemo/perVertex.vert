
varying vec4 pixColor;

vec4 calcDiffuse(vec4 lightdiffuse, vec3 n, vec4 s, vec4 matdiffuse)
{
  return lightdiffuse * max(0.0, dot(n, s.xyz)) * matdiffuse;
}

vec4 calcSpecular(in float shininess, in vec4 lightspec, in vec4 matspec, in vec3 r, in vec3 v)
{
  vec4 spec;
  if (shininess != 0.0) 
  {
    spec = lightspec * matspec * pow(max(0.0, dot(r, v)), shininess);
  } 
  else 
  {
    spec = vec4(0.0, 0.0, 0.0, 0.0);
  }
  return spec;
}

void main() 
{
  //get information about our vertex in eye coordinates
  vec3 normal = gl_NormalMatrix * gl_Normal; //the normal in eye coordinates
  vec4 pos = gl_ModelViewMatrix * gl_Vertex; //the position in eye coordinates

  //get info about the material of the object (set in OpenGL with glMaterial)
  vec4 matdiffuse = gl_FrontMaterial.diffuse;
  vec4 matspec = gl_FrontMaterial.specular;
  float shininess = gl_FrontMaterial.shininess;

  //get info about the light (set in OpenGL with glLight)
  vec4 lightdiffuse = gl_LightSource[0].diffuse;
  vec4 lightspec = gl_LightSource[0].specular;
  vec4 lpos = gl_LightSource[0].position; //light position in eye coordinates
  
  //calculate vectors needed for lighting model
  vec4 s = normalize(lpos - pos); //vector from vertex to light
  vec3 n = normalize(normal); //the normal vector pointing away from the vertex
  vec3 r = normalize(-reflect(s.xyz, n)); //vector from vertex pointing halfway between light and camera
  vec3 v = normalize(-pos.xyz); //vector pointing from vertex to camera
   
  //calculate SPECULAR lighting  
  vec4 specular = calcSpecular(shininess, lightspec, matspec, r, v);

  //calculate DIFFUSE lighting 
  vec4 diffuse  = calcDiffuse(matdiffuse, n, s, lightdiffuse);  
  
  //add up the lighting terms to get the color at the vertex
  pixColor = diffuse + specular;

  //calculate position in clip space using fixed-functionality (ie multiplying the vertex through the modelview and projection matrices)
  gl_Position = ftransform(); //the position in clip coordinates

}



