#include "bluelabel.h"
//#include <stdio.h>

BlueLabel::BlueLabel(QWidget *parent) :
    QLabel(parent)
{
}

void BlueLabel::concatenate(QString x)
{
   QString w = text();

   int l = w.length()+1;
    //printf("%d ", l);


   if (l<=10)
   {      
       w = w.append(x);
       setText(w);
   }
   else
   {
       setText(x);

   }
}
