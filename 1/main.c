#include "framework.h"
#include "logo.h"

int main(int argc, char const *argv[]) {
  if (init() == FALSE) return(FALSE);

  DrawImage( my_window->RPort, &amiga_logo, 3, 11 );
  
  close();
  return 0;
}
