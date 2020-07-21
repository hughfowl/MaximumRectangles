
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;


struct Corner {
        //x, y
        int x;
        int y;

        Corner() : x(0), y(0) {}
        Corner(int x, int y ) : x(x), y(y) {}

    };

struct Rectangle {
        //x, y, w, h
        int X;
        int Y;
        int W;
        int H;
        Corner corners[4];

        Rectangle(){
            X = 0;
            Y = 0;
            W = 0;
            H = 0;
            for (int i = 0; i < 4; i++)
            {
                corners[i].x = 0;
                corners[i].y = 0;
            }
        }
        Rectangle(int x, int y, int w, int h){
            X = x;
            Y = y;
            W = w;
            H = h;
            corners[0].x = x;
            corners[0].y = y;
            corners[1].x = x+w;
            corners[1].y = y;
            corners[2].x = x+w;
            corners[2].y = y+h;
            corners[3].x = x;
            corners[3].y = y+h;

        }

 };

struct ProSlope
{
    double slope;
    int index;
    int number;
    ProSlope() {
        slope = 0;
        index = 0;
        number = 0;
    }
    ProSlope(double s, int i, int n) {
        slope = s;
        index = i;
        number = n;
    }
};

bool compare(const ProSlope &x, const ProSlope &y) {
    return x.slope > y.slope;
}

double utilSlope(Corner p1, Corner p2);

int main(int argc, const char * argv[])
{

    for (int inn = 1; inn<argc; inn++) {
    
    //open file
    ifstream afile;
    afile.open(argv[inn],ios::out | ios::in);
    if (afile.fail())
    {
        cout << "File does not exist" << endl;
        cout << "Exit program" << endl;
        return 0;
    }
    int numTotalRect = 0;
    afile >> numTotalRect;
    int rectTemp1;
    int rectTemp2;
    int rectTemp3;
    int rectTemp4;
    std::vector<Rectangle> rectangles;

    while (!afile.eof())
    {
        afile >> rectTemp1;
        afile >> rectTemp2;
        afile >> rectTemp3;
        afile >> rectTemp4;

        rectangles.push_back(Rectangle(rectTemp1, rectTemp2, rectTemp3, rectTemp4));

    }

    //Algo start
    int maxRectLineOfSightCount = 0;

    for (int i = 0; i < rectangles.size(); i++)
    {
        Rectangle indexRect = rectangles[i];
        for (int j = 0 ; j < 4 ; j++) {
           Corner indexConer = indexRect.corners[j];
           std::vector<ProSlope> slopeList;
           for (int k = 1; k < rectangles.size(); k++)
           {
               Rectangle otherRect = rectangles[k];
               std::vector<double> slopeParams;
               for (int l = 0; l < 4; l++)
               {
                   Corner theConer = otherRect.corners[l];
                   double slope = utilSlope(indexConer, theConer);
                   slopeParams.push_back(slope);
               }
               std::sort(slopeParams.rbegin(), slopeParams.rend());
               if (slopeParams[1] >= 270 && slopeParams[2] <= 90)
               {
                   slopeList.push_back(ProSlope(slopeParams[0], k, 0));
                   slopeList.push_back(ProSlope(slopeParams[1], k, 3));
                   slopeList.push_back(ProSlope(slopeParams[2], k, 0));
                   slopeList.push_back(ProSlope(slopeParams[3], k, 3));
               }
               else
               {
                   slopeList.push_back(ProSlope(slopeParams[0], k, 0));
                   slopeList.push_back(ProSlope(slopeParams[1], k, 1));
                   slopeList.push_back(ProSlope(slopeParams[2], k, 2));
                   slopeList.push_back(ProSlope(slopeParams[3], k, 3));
               }
           }
           std::sort(slopeList.begin(),slopeList.end(),compare);
           //int slopeId = 0;
           //int orderId = 2;
           int rectLineOfSightCount = 0;
           int rectExitCount = 0;
           double rectExitSlope = -1;
           for (int k2 = 0; k2 < slopeList.size(); k2++)
           {
               if (slopeList[k2].number == 0)
               {
                   rectLineOfSightCount += 1;
               }
               if (rectExitCount > 0 && slopeList[k2].slope != rectExitSlope)
               {
                   rectExitSlope = slopeList[k2].slope;
                   rectLineOfSightCount -= rectExitCount;
                   rectExitCount = 0;
               }
               if (slopeList[k2].number == 3)
               {
                   rectExitCount += 1;
                   rectExitSlope = slopeList[k2].slope;
               }
               if (maxRectLineOfSightCount < rectLineOfSightCount)
               {
                   if (rectLineOfSightCount == 1) {
                       maxRectLineOfSightCount = rectLineOfSightCount;
                   }else{
                       maxRectLineOfSightCount = rectLineOfSightCount;
                       maxRectLineOfSightCount = maxRectLineOfSightCount-1 ;
                   }
                   
               }
           }
        }
    }

    //Algo end

    afile.close();
    
    cout << maxRectLineOfSightCount << endl;
    }
    return 0;
}


double utilSlope(Corner p1, Corner p2) {
    double x1 = p1.x;
    double y1 = p1.y;
    double x2 = p2.x;
    double y2 = p2.y;
    double rad = 0;
    double slope = 0;

    rad = atan2(y2 - y1, x2 - x1);

    double Rad_to_deg = 45.0 / atan(1.0);
    slope = rad * Rad_to_deg;
    if (slope < 0.0) {
        slope += 360.0;
    }

    return slope;

}
