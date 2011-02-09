#include <stdlib.h>
#include <stdio.h>

//declaring and dereferencing pointers
void pointer1(void) {
  int z = 11;
  printf("value of 'z' = %d\n", z);
  printf("address of 'z' = %p\n", &z);

  int *a = &z;
  //a points to the address of z (which now contains the int 11)
  printf("value in space that 'a' points to = %d\n", *a);


  int *b = &z;
  //a and b both point to the same place
  *b += 1;
  //so now we have updated the value that z points to as well...
  printf("value in space that 'a' points to = %d\n", *a);
  printf("value in space that 'b' points to = %d\n", *b);
  printf("value of 'z' = %d\n", z);


  float *c = &z;
  //uh oh, this will compile (with warnings) but will cause problems...
  *c += 1;
  //what have we done to z???
  printf("value of 'z' = %d\n", z);
}


//allocating memory
void pointer2(void) {
   int a = 1;
   int b = 2;
   int c = 3;
  
   //allocate memory using "malloc"...
   int *ints = (int *) malloc (sizeof(int) * 3);
   //now we have space available to place 3 ints
   
   //lets keep track of the beginning of this space...
   int *start = ints;
   int *start2 = ints;

   //pointer arithmetic... ++ jumps to the next space for an int
   *ints = a;
   ints++;
   *ints = b;
   ints++;
   *ints = c;

   //now we have filled up our memory, and we are still pointing to the third space...
   printf("ints = %d\n", *ints); 
   
   //but if we dereference the start of the memory, we will see the first space...
   printf("start = %d\n", *start); 

   //and we can loop through the space in different ways...
   
   //1. using pointer arithemtic, and then dereferencing that pointer:
   int i;
   for (i = 0; i < 3; i++) {
    printf("int at memory pos %d = %d\n", i, *start); 
    start++; 
   }
    //now start points to somewhere undefined...

   //2. using array notation (which has the benefit of not altering the location of our pointer)
   int j;
   for (j = 0; j < 3; j++) {
    printf("int at memory pos[%d] = %d\n", j, start2[j]); 
   }
    //start2 still points to the beginning of our allocated memory space...

   //when you are done with the memory, you can free it so that it is available once again...
   //this is super important if you are allocating within a loop, else it will cause a leak.
   free(start2);
   //also good idea to nullify your pointers so you don't inadvertantly try to use them again before re-mallocing space for them...
   ints = NULL;
   start = NULL;
   start2 = NULL;
}

//passing by value
void foo1(int a) {
  //this a is a brand new "a" with a brand new space in memory
  a += 1;
}

int func1(void) {
  int a = 5;
  foo1(a); //passing in a
  printf("a now = %d\n", a); //a = 5
}

//passing by reference
void foo2(int *a) {
  //this a is a copy of the address (ie a new pointer), which still lets us get to the variable in memory
  *a += 1;
}

int func2(void) {
  int a = 5;
  foo2(&a); //pass in the address of variable a (i.e., a pointer to a)
  printf("a now = %d\n", a); //a = 6
}


struct info {
  int i;
  float f;
  char *s;
};

void structs(void) {
  struct info info1; //make a new instance of the info struct
  info1.i = 10;
  info1.f = 9.99;
  info1.s = "abcd";
  //now we have put data into the struct...
  printf("i = %d\n", info1.i);
  printf("f = %f\n", info1.f);
  printf("s = %s\n", info1.s);
}

int main(int argc, char** argv) {

  int i;    
  for(i = 0; i < argc; i++)
  {
    printf( "argg %d: %s\n", i, argv[i] );
  }

  int which = atoi(argv[1]);

  switch(which) {
    case 1: pointer1(); break;
    case 2: pointer2(); break;
    case 3: func1(); break;
    case 4: func2(); break;
    case 5: structs(); break;
  }
}


