

GLuint initializeShaders(char *vFile, char *fFile);

void printLog(GLuint obj)
{
  int infologLength = 0;
  int maxLength;

  if(glIsShader(obj)) {
    glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
  }
  else {
    glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
  }

  char infoLog[maxLength];

  if (glIsShader(obj)) {
    glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
  }
  else {
    glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
  }
  
  if (infologLength > 0) {
    printf("%s\n",infoLog);
  }
}

void getGlVersion(void) {
  const char *verstr = (const char *) glGetString(GL_VERSION);
  int major = 0;
  int minor = 0;
  sscanf(verstr, "%d.%d", &major, &minor);
  printf("OpenGL version = %d.%d\n", major, minor);
}

void getGlslVersion() {
  const char *verstr = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
  int major = 0;
  int minor = 0;
  sscanf(verstr, "%d.%d", &major, &minor);
  printf("GLSL version = %d.%d\n", major, minor);
}


char* loadShader(const char *path) {
  FILE *fd;
  long len, r;
  char *str;

  if (!(fd = fopen(path, "r")))
  {
    fprintf(stderr, "Can't open file '%s' for reading\n", path);
    return NULL;
  }

  fseek(fd, 0, SEEK_END);
  len = ftell(fd);

  printf("File '%s' is %ld long\n", path, len);

  fseek(fd, 0, SEEK_SET);

  if (!(str = malloc(len * sizeof(char))))
  {
    fprintf(stderr, "Can't malloc space for '%s'\n", path);
    return NULL;
  }

  r = fread(str, sizeof(char), len, fd);

  str[r - 1] = '\0'; // Shader sources have to term with null

  fclose(fd);

  return str;
}


GLuint initializeShaders(char *vFile, char *fFile) {
  getGlVersion();
  getGlslVersion();

  /* load vertex and fragment shader */
  const char *vsSource = loadShader(vFile);
  const char *fsSource = loadShader(fFile);

  //printf("\n *** vertex shader *** \n%s\n", vsSource);
  //printf("\n *** fragment shader *** \n%s\n", fsSource);
  
  /* Compile and load the program */

  GLuint vs, /* Vertex Shader */
	 fs, /* Fragment Shader */
	 sp; /* Shader Program */

  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vsSource, NULL);
  glCompileShader(vs);
  printLog(vs);

  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fsSource, NULL);
  glCompileShader(fs);
  printLog(fs);

  sp = glCreateProgram();
  glAttachShader(sp, vs);
  glAttachShader(sp, fs);
  glLinkProgram(sp);
  printLog(sp);
  //printf("sp = %d\n", sp);

  return sp;
}



