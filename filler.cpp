/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace. 
 *
 */
//#include "filler.h"

animation filler::fillSolidDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                                 double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    solidColorPicker solidCP(fillColor);
    return fill<Stack>(img, x, y, solidCP, tolerance, frameFreq);
}

animation filler::fillGridDFS(PNG& img, int x, int y, HSLAPixel gridColor,
                                int gridSpacing, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gridColorPicker gridCP(gridColor, gridSpacing);
    return fill<Stack>(img, x, y, gridCP, tolerance, frameFreq);

}

animation filler::fillGradientDFS(PNG& img, int x, int y,
                                    HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                    int radius, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gradientColorPicker gradCP(fadeColor1, fadeColor2, radius, x, y);
    return fill<Stack>(img, x, y, gradCP, tolerance, frameFreq);

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
    /**
     * @todo Your code here! 
     */
    solidColorPicker solidCP(fillColor);
    return fill<Queue>(img, x, y, solidCP, tolerance, frameFreq);
}

animation filler::fillGridBFS(PNG& img, int x, int y, HSLAPixel gridColor,
                                int gridSpacing, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gridColorPicker gridCP(gridColor,gridSpacing);
    return fill<Queue>(img, x, y, gridCP, tolerance, frameFreq);
}

animation filler::fillGradientBFS(PNG& img, int x, int y,
                                    HSLAPixel fadeColor1, HSLAPixel fadeColor2,
                                    int radius, double tolerance, int frameFreq)
{
    /**
     * @todo Your code here! 
     */
    gradientColorPicker gradientCP(fadeColor1,fadeColor2,radius,x,y);
    return fill<Queue>(img, x, y, gradientCP, tolerance, frameFreq);
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

    animation anim;
     
    OrderingStructure<HSLAPixel*> points;
    OrderingStructure<int> xC;
    OrderingStructure<int> yC;
    int frameCount = 0;     

    vector<vector<bool>> processed;
    for (unsigned int i = 0; i < img.height(); i++) {
        vector<bool> horizontal;
        for (unsigned int j = 0; j < img.width(); j++) {
            horizontal.push_back(false);
        }
        processed.push_back(horizontal);
    }

    HSLAPixel newColor = fillColor(x, y);
    HSLAPixel* currPix = img.getPixel(x, y);
    HSLAPixel strPix =* currPix;
    *currPix = newColor;

    xC.add(x);
    yC.add(y);
    points.add(currPix);
    processed[y][x] = true;

    if (frameCount % frameFreq == 0) {
        anim.addFrame(img);
    }
 
    while (!points.isEmpty()) {
        HSLAPixel* curr = points.remove();
        unsigned int xCurr = xC.remove();
        unsigned int yCurr = yC.remove();

        HSLAPixel* rightFriend = img.getPixel(xCurr + 1, yCurr);
        if (rightFriend->dist(strPix) <= tolerance && xCurr + 1 < img.width() && yCurr < img.height() && !processed[yCurr][xCurr + 1]) {

            processed[yCurr][xCurr + 1] = true;

            HSLAPixel colPoint = fillColor(xCurr + 1, yCurr);
            *rightFriend = colPoint;

            points.add(rightFriend);
            xC.add(xCurr + 1);
            yC.add(yCurr);

            frameCount++;
            if (frameCount % frameFreq == 0) {
            anim.addFrame(img);
            }
        }

        HSLAPixel* downFriend = img.getPixel(xCurr, yCurr + 1);
        if (downFriend->dist(strPix) <= tolerance && xCurr < img.width() && yCurr + 1 < img.height() && !processed[yCurr + 1][xCurr]) {
            processed[yCurr + 1][xCurr] = true;
            HSLAPixel colPoint = fillColor(xCurr, yCurr + 1);
            *downFriend = colPoint;

            points.add(downFriend);
            xC.add(xCurr);
            yC.add(yCurr + 1);
            frameCount++;
            if (frameCount % frameFreq == 0){
            anim.addFrame(img);
            }
        }

        HSLAPixel* leftFriend = img.getPixel(xCurr - 1, yCurr);
        if (leftFriend->dist(strPix) <= tolerance && xCurr - 1 < img.width() && yCurr < img.height() && !processed[yCurr][xCurr - 1]) {
            processed[yCurr][xCurr - 1] = true;
            HSLAPixel colPoint = fillColor(xCurr - 1, yCurr);
            *leftFriend = colPoint;

            points.add(leftFriend);
            xC.add(xCurr - 1);
            yC.add(yCurr);

            frameCount++;
            if (frameCount % frameFreq == 0) {
            anim.addFrame(img);
            }
        }

        HSLAPixel* upFriend = img.getPixel(xCurr, yCurr - 1);
        if (upFriend->dist(strPix) <= tolerance && xCurr < img.width() && yCurr - 1 < img.height() && !processed[yCurr - 1][xCurr]) {
            processed[yCurr - 1][xCurr] = true;
            HSLAPixel colPoint = fillColor(xCurr, yCurr - 1);
            *upFriend = colPoint;

            points.add(upFriend);
            xC.add(xCurr);
            yC.add(yCurr - 1);
            frameCount++;
            if (frameCount % frameFreq == 0) {
            anim.addFrame(img);
            }
        }   
    }
    
    anim.addFrame(img);
    return anim;
} 
