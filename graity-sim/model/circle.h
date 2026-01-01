#ifndef CIRCLE_H
#define CIRCLE_H

#include "../glad/glad.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include<time.h>

class Circle{
  unsigned int VAO, VBO, EBO;
  int res;
  float centerx , centery, radius; 
  float mass;
  float velocity[2] = {0.0f, 0.0f};
  float aceleration[2] = {0.0f, -10.0f};
  float centerxInitial, centeryInital;

  float tyFinal, txFinal;
  float tyInitial = 0.0f, txInitial = 0.0f;

  bool changeYdir = false, changeXdir = false;

  public:

    Circle(int _res, float _centerx, float _centery, float _radius){
      res = _res;
      centerx = _centerx;
      centery = _centery;
      radius = _radius;

      centerxInitial = centerx;
      centeryInital = centery;

      calculateTyFinal();
      calculateTxFinal();
      setVertexs();

    }

    void changePosition(){
      
      // if (!changeYdir){
      //   tyInitial += 0.01f;
      //   if(tyInitial >= tyFinal) changeYdir = true;
      // }else{
      //   tyInitial -= 0.01f;
      //   if(tyInitial <= 0.0f) changeYdir = false;
      // }

      centerx += velocity[0];
      centery += velocity[1];
    
      // centery = centeryInital - ( ( aceleration[1] * pow(tyInitial, 2) ) / 2 );
      // centerx += 0.01;
      // std::cout << "centery: " << centery << std::endl;
      // std::cout << "tInital: " << tyInitial << std::endl;

    }

    void acelerate(){

      if(centery-radius <= -1){

        velocity[1] = -velocity[1];

      }

      velocity[1] += aceleration[1]/9000;

      

      std::cout << "velocidade y: " << velocity[1] << std::endl;
      std::cout << "centery: " << centery << std::endl; 

      velocity[0] += aceleration[0]/10000;
    }



    void use(){

      setVertexs();

      glBindVertexArray(VAO);

      glDrawElements(GL_TRIANGLES, res*3, GL_UNSIGNED_INT, 0);

      // std::cout <<  centerx << std::endl;

    }

    void deleteBuffers(){
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);
    }

    float calculateGForce(float otherMass, float distance){
      float G = 6.67 * pow(10, -11);

      return ( G * mass * otherMass ) / (distance * distance);
    }

  private:
    void setVertexs(){
      int count = 3;
      float angleRad = 2.0f * 3.14159265359 / res;

      // std::cout << "angle: " << angleRad << std::endl;
      
      int indices[res*3];
      
      float vertices[(res+1)*3];

      float x = (cos(angleRad) * radius) + centerx;
      float y = (sin(angleRad) * radius) + centery;
      float z = 0.0f;

      // std::cout << "coordinates (x, y, z): " << x << " " << y << " " << z << "\n" << std::endl;

      vertices[0] = centerx;
      vertices[1] = centery;
      vertices[2] = z;

      for(int i = 0; i < res; i++){

        vertices[count] = x;
        vertices[count+1] = y;
        vertices[count+2] = z;

        // std::cout << "old coordinates: " << x << " " << y << std::endl;

        x = (cos(i*angleRad) * radius) + centerx;
        y = (sin(i*angleRad) * radius) + centery;

        count = count + 3;


        // std::cout << "new coordinates: " << x << " " << y << std::endl;
      
      }

      count = 0;
      int aux = 1;
      for(int i = 0; i < res; i++){
        
        indices[count] = 0;

      
        if(aux == res){
          indices[count+1] = aux;
          indices[count+2] = 1;  
        }else{
          indices[count+1] = aux;
          indices[count+2] = aux+1;

        }
        

        count = count+3;
        aux = aux + 1;
      }

      // std::cout << "vertices number: " << sizeof(vertices)/sizeof(float) << std::endl;
      
      // showVertices(vertices);

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

    }

    void showVertices(float vertices[]){
      int size = (res*3*3);
      int aux = 1;
      for(int i = 0; i < size; i=i+3){
        std::cout << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << std::endl;

        if(aux%3 == 0 && i != 0) {
          std::cout << "------------------------------------" << std::endl;
        }
        aux++;
      }
    }

    void calculateTxFinal(){
      txFinal = sqrt( pow(velocity[0], 2) + 2 * aceleration[0] * (centerx + 1 - radius) ) / aceleration[0];
      std::cout << "txFinal: " << txFinal << std::endl;

    }

    void calculateTyFinal(){
      tyFinal = sqrt( pow(velocity[1], 2) + 2 * aceleration[1] * (centery + 1 - radius) ) / aceleration[1];
      // std::cout << "tyFinal: " << tyFinal << std::endl;
    }

};

#endif