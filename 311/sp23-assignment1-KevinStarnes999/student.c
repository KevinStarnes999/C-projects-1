#include "student.h"

int largest(int array[], int length) {
  int largest = array[0];
  for (int i = 0; i < length; i++){
    if (array[i] > largest){
      largest = array[i];
    }
  }
  return largest;
}

int sum(int array[], int length) {
  int sum1 = 0;
  for(int i = 0; i < length; i++){
    sum1 = sum1 + array[i];
  }
  return sum1;

}

void swap(int *a, int *b) {
  int temp;
  temp = *a;
  *a = *b;
  *b = temp; 
}

void rotate(int *a, int *b, int *c) {
  int temp;
  int temp1;
  int temp2;
  temp = *c;
  temp1 = *b;
  temp2 = *a;
  *c = *b;
  *b = *a;
  *a = *c;
  *a = temp;
  *c = temp1;
  *b = temp2;
}

void sort(int array[], int length) {
  int x;
  for(int i = 0; i < length; i++)
    {
      for(int j = i + 1; j < length; j++)
	{
	  if(array[i] > array[j])
	  {
	    x = array[i];
	    array[i] = array[j];
	    array[j] = x;
	  }
	 }
     }
}
void double_primes(int array[], int length) {  //RUN TIME IS LONG SO PLEASE WAIT AND NOT THINK ITS AN INFINITE LOOP
  int b = 2;
  int c = 3;
  int d = 5;
  int e = 7;

   for (int i = 0; i < length; i++){
    
     
     if(array[i] % c == 0 && array[i] != c){
     }
     else if(array[i] % b == 0 && array[i] != b){
     }
       
     else if(array[i] % d == 0 && array[i] != d){
     }
  
     else if(array[i] % e == 0 && array[i] != e){
     }
     else if(array[i] != 1){
       array[i] = array[i] *2;
     }if(array[i] == 2)
	array[i] = array[i] * 2;
   }
}
int help(int a, int b){
  for(int x = 0; x < b-1; x++){
    a = a * a;
    
  }
  return a;
}

void negate_armstrongs(int array[], int length) {
  for(int i = 0; i < length; i++){
    int num1 = array[i] % 10;
    int temp = array[i] / 10;
    int num2 = temp % 10;
    temp = temp / 10;
    int num3 = temp % 10;
    int num4 = temp / 10;
    for( int x = 4; x >1; x++){
      if(num4 == 4){
	int x1 = help(num1,4);
	int x2 = help(num2,4);
	int x3 = help(num3,4);
	int x4 = help(num4,4);
	int sum = x1+x2+x3+x4;
	if(sum == array[i] && array[i] > 0)
	  array[i] = array[i] * -1;
	else{
	  if(array[i] > 0)
	    array[i] = array[i] * -1;
	}
      }  if(num3 == 3){
	int x1 = help(num1,3);
	int x2 = help(num2,3);
	int x3 = help(num3,3);
	int sum = x1+x2+x3;
	if(sum == array[i] && array[i] > 0)
	  array[i] = array[i] * -1;
	else{
	  if(array[i] > 0)
	    array[i] = array[i] * -1;
	}
      }  if(num2 == 2){
	int x1 = help(num1,2);
	int x2 = help(num2,2);
	int sum = x1+x2;
	if(sum == array[i] && array[i] > 0)
	  array[i] = array[i] * -1;
      }	else{
	  if(array[i] > 0)
	    array[i] = array[i] * -1;
      
    
   
      }
  
  }
}
}
