attribute float opac;
varying float opacity;
void main()
{
    opacity = opac;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord1;
    gl_TexCoord[2] = gl_MultiTexCoord2;
    gl_Position = ftransform();
    
}
