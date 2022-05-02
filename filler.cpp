/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. 
 *
 */
#include "filler.h"

animation filler::fillSolidDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                 double tolerance, int frameFreq)
{
    solidColorPicker a(fillColor);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillGridDFS(PNG& img, int x, int y, HSLAPixel gridColor,
                                int gridSpacing, double tolerance, int frameFreq)
{
    gridColorPicker a(gridColor, gridSpacing);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);

}

animation filler::fillGradientDFS(PNG& img, int x, int y,
                                    HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                    int radius, double tolerance, int frameFreq)
{
    gradientColorPicker a(fadeColor1, fadeColor2, radius, x, y);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);

}


animation filler::fillRainDFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    /**
     *  Our code here!
     */

    rainbowColorPicker a(freq);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillSolidBFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                 double tolerance, int frameFreq)
{
    solidColorPicker a(fillColor);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillGridBFS(PNG& img, int x, int y, HSLAPixel gridColor,
                                int gridSpacing, double tolerance, int frameFreq)
{
    gridColorPicker a(gridColor, gridSpacing);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillGradientBFS(PNG& img, int x, int y,
                                    HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                    int radius, double tolerance, int frameFreq)
{
    gradientColorPicker a(fadeColor1, fadeColor2, radius, x, y);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillRainBFS(PNG& img, int x, int y,
                                    long double freq, double tolerance, int frameFreq)
{
    /**
     * Our code here!
     */
    rainbowColorPicker a(freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor,
                       double tolerance, int frameFreq)
{
    OrderingStructure<vector<int>> pts;
    map<vector<int>, bool> checkedPts;
    vector<int> point = {x, y};
    animation animation;
    pts.add(point);
    PNG ogImg = img;
    HSLAPixel* pixel0 = img.getPixel(x,y);
    HSLAPixel startColor = *pixel0;
    *pixel0 = fillColor(x,y);
    animation.addFrame(img);
    int height = img.height();
    int width = img.width();
    checkedPts[point] = true;

    
    int currX;
    int currY;
    int filled = 0;
    while (!pts.isEmpty()) {
        vector<int> currPoint = pts.remove();
        currX = currPoint[0];
        currY = currPoint[1];

        int rX = currX+1;
        int rY = currY;
        int dX = currX;
        int dY = currY+1;
        int lX = currX-1;
        int lY = currY;
        int uX = currX;
        int uY = currY-1;

        vector<int> rightPoint = {rX, rY};
        vector<int> leftPoint = {lX, lY};
        vector<int> downPoint = {dX, dY};
        vector<int> upPoint = {uX, uY};
        
        // RIGHT POINT
        if (filler::check_valid(rightPoint, ogImg, startColor, checkedPts, tolerance)) {
            HSLAPixel * rPixel = img.getPixel(rX, rY);
            HSLAPixel rColor = *rPixel;
            *rPixel = fillColor(rX, rY);
            filled++;
            checkedPts[rightPoint] = true;
            pts.add(rightPoint);
            if (filled % frameFreq == 0) {
                animation.addFrame(img);
            }
        }
        
        // DOWN POINT
        if (filler::check_valid(downPoint, ogImg, startColor, checkedPts, tolerance)) {
            HSLAPixel * dPixel = img.getPixel(dX, dY);
            *dPixel = fillColor(dX, dY);
            filled++;
            checkedPts[downPoint] = true;
            pts.add(downPoint);
            if (filled % frameFreq == 0) {
                animation.addFrame(img);
            }
        }
        
        // LEFT POINT
        if (filler::check_valid(leftPoint, ogImg, startColor, checkedPts, tolerance)) {
            HSLAPixel * lPixel = img.getPixel(lX, lY);
            *lPixel = fillColor(lX, lY);
            filled++;
            checkedPts[leftPoint] = true;
            pts.add(leftPoint);
            if (filled % frameFreq == 0) {
                animation.addFrame(img);
            }
        }
        
        // UP POINT
        if (filler::check_valid(upPoint, ogImg, startColor, checkedPts, tolerance)) {
            HSLAPixel * uPixel = img.getPixel(uX, uY);
            *uPixel = fillColor(uX, uY);
            filled++;
            checkedPts[upPoint] = true;
            pts.add(upPoint);
            if (filled % frameFreq == 0) {
                animation.addFrame(img);
            }
        }    
    }

    animation.addFrame(img);
    /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. 
     *
     * To begin the algorithm, you simply:
     * a) place the given point in the ordering structure, 
     * marking it processed (the way you mark it is a design 
     * decision you'll make yourself).
     *
     * b) We have a choice to either change the color, if appropriate, 
     * when we * add the point to the OS, or when we take it off. 
     * In our test cases, * we have assumed that you will change 
     * the color when a point is added to the structure. 
     *
     * c) Add the first image to the animation (it's convenient to 
     * think of this as the 0th image). 
     *
     * THEN, until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        a.    add its unprocessed neighbors whose color values are 
     *              within (or equal to) tolerance distance from the 
     *              center's color, to the ordering structure, set their
     *              new color, and mark them as processed.
     *        b.    if it is an appropriate frame, send the current 
     *              PNG to the animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore 
     *        neighboring pixels in some order.
     *
     *        While the order in which you examine neighbors does 
     *        not matter for a proper fill, you must use the same 
     *        order as we do for your animations to come out like 
     *        ours! The order you should put neighboring pixels 
     *        **ONTO** the queue or stack is as follows:
     *        RIGHT(+x), DOWN(+y), LEFT(-x), UP(-y). 
     *        IMPORTANT NOTE: *UP* here means towards the top of 
     *        the image, so since an image has smaller y coordinates 
     *        at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction. 
     *
     *        To reiterate, when you are exploring 
     *        (filling out) from a given pixel, you must first try 
     *        to fill the pixel to
     *        it's RIGHT, then the one DOWN from it, then to the LEFT and
     *        finally UP. If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then 
     *        again after the 8th pixel, etc.  You must only add 
     *        frames for the number of pixels that have been filled, 
     *        not the number that have been checked. So if frameFreq 
     *        is set to 1, a pixel should be filled every frame.
     *
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the 
     *        fill, and it will be the one we test against.
     */
    return animation;
}

bool filler::check_valid(vector<int>& point, PNG& img, HSLAPixel& startColor, map<vector<int>, bool> & checkedPts, double tolerance) {
    if (point[0] < 0 || (unsigned int) point[0] >= img.width() || point[1] < 0 || (unsigned int) point[1] >= img.height()) return false;
    if (checkedPts[point]) return false;
    if (startColor.dist(*img.getPixel(point[0], point[1])) > tolerance) return false;
    return true;
}

/**
 * @todo INFINITE LOOP
 * 
 */