int i = 0;
int k = 0;
int off = 5;

void inc(void){
  i += off;
  k += i;
}


int main(void){
  while (1) {
    inc();
  }
}

