struct face {
  int numSides;
  int v1, t1, n1, v2, t2, n2, v3, t3, n3, v4, t4, n4;
};

struct p3f {
  float x;
  float y;
  float z;
};

void keyboard(unsigned char key, int x, int y);
void keySpecial(int key, int x, int y);
void idle(void);
void display(void);
void reshape(int width, int height);
void timer(int value);
void print_face(struct face f);
void print_faces(void);
void print_p3f(struct p3f p);
void scanVertex(char line[], float scale);
void scanNormal(char line[]);
void scanTexCoord(char line[]);
void scanFace(char line[], int numSides);



