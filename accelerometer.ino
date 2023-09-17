/* Sketch for accelerometer readings. Detects if device is stationary of moving
/  Author: Markus Andersson, 11/2022
*/

#include <Ewma.h>

double time_now = 0;
int x_now, y_now, z_now;
int x_der, y_der, z_der;
int tot_der;

void read_raw_values(double* x, double* y, double* z){ // reads raw values from accelerometer
  *x = analogRead(X_PIN);      
  *y = analogRead(Y_PIN);   
  *z = analogRead(Z_PIN);
}

bool moving(){ // returns true if person is walking
  unsigned int der_sum = 0;
  // sum together 50 readings
  for(unsigned int i = 0; i < 50; i++){
    read_raw_values(&x_raw, &y_raw, &z_raw); // read raw values

    // filter raw values
    x_now = exp_filter_x.filter(x_raw);
    y_now = exp_filter_y.filter(y_raw);
    z_now = exp_filter_z.filter(z_raw);

    time_now = micros(); 
    
    // calculate the derivative of the acc. values
    double elapsed_time = (time_now - time_old) / 1000000.0;

    int x_der = (x_now - x_old) / elapsed_time;
    int y_der = (y_now - y_old) / elapsed_time;
    int z_der = (z_now - z_old) / elapsed_time;
   
    // root sum of squares = total derivative
    unsigned int tot_der = sqrt(pow(x_der, 2) + pow(y_der, 2) + pow(z_der, 2));
    der_sum += tot_der;

    time_old = time_now;
    x_old = x_now;
    y_old = y_now;
    z_old = z_now;
  } 
  if(der_sum){ //if sum != 0 -> device is moving
    return true;
  }
  else{
    return false;
  }
  
}




