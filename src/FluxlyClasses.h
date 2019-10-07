//
//  FluxlyClasses.h
//  Custom C++ classes for Fluxly, Musical Physics Looper
//
//  Created by Shawn Wallace on 11/14/17.
//  See the LICENSE file for copyright information

#ifndef customClasses_h
#define customClasses_h

#define IOS
//#define ANDROID

//#define FLUXLY_FREE (0)
#define FLUXLY_STANDARD (1)
//#define FLUXLY_PRO (2)

#define MAIN_MENU (0)
#define SAMPLE_MENU (1)

#define SCOPE_SIZE (256)

#include "ofxXmlSettings.h"

//--------------------------------------------

class FluxlyCircle : public ofxBox2dCircle {
public:
    FluxlyCircle() {
    }
    
    ofColor color;
    /* Color type:
     1: ff0000  or  f62394
     2: 8b20bb  or  0024ba
     3: 007ac7  or  00b3d4
     4: 01b700  or  83ce01
     5: fffa00  or  ffcf00
     6: ffa600  or  ff7d01
     */
    
    int id;
    int w;
    int x;
    int y;
    int bx;
    int by;
    int bw;
    int bh;
    string bLabel;
    string bValue;
    int density;
    int bounce;
    int friction;
    int senseType;   // 0 = angular velocity, 1 = x position, 2 = y position
    int displayW;
    bool eyeState = false;
    bool onOffState = false;
    int eyePeriod = 100;
    int type;
    int origType;
    int nJoints = 0;
    int connections[4];
    int count = 0;
    //int stroke = 1;
    int prevState =0;
    float dampingX;
    float dampingY;
    bool sendOn = false;
    bool sendOff = false;
    bool sendControl = false;
    bool spinning = false;
    bool wasntSpinning = true;
    bool touched = false;
    int touchId = -1;
    float rotation = 0.0;
    
    int soundWaveStep = 2;
    int soundWaveH = 100;
    int soundWaveStart = -512;
    int maxAnimationCount = 150;
    int animationStep = 6;
    
    float tempo = 0;
    float prevTempo = 0;
    float prevX = 0;
    float prevY = 0;
    int instrument;
    
    string filename;
    
    vector<float> scopeArray;
    
   // ofTrueTypeFont vag;
    
    b2BodyDef * def;
    
    ofImage myEyesOpen;
    ofImage myEyesClosed;
    ofImage grayOverlay;
    ofImage spriteImg;
    ofImage blueGlow;
    
    float retinaScale;
    
    void init() {
        myEyesOpen.load("eyesOpen.png");
        myEyesClosed.load("eyesClosed.png");
        blueGlow.load("blueGlow.png");
        //vag.load("vag.ttf", 9);
        origType = type;
        setMesh();
        soundWaveStart = soundWaveStart*retinaScale;
        //soundWaveStep = soundWaveStep * retinaScale;
        
    }
    
    void setMesh() {
        filename = "mesh" + to_string(type) + ".png";
        
        switch (type % 12) {
            case 0:
                color = ofColor::fromHex(0xff0000);
                break;
            case 1:
                color = ofColor::fromHex(0xf62394);
                break;
            case 2:
                color = ofColor::fromHex(0x8b20bb);
                break;
            case 3:
                color = ofColor::fromHex(0x0024ba);
                break;
            case 4:
                color = ofColor::fromHex(0x007ac7);
                break;
            case 5:
                color = ofColor::fromHex(0x00b3d4);
                break;
            case 6:
                color = ofColor::fromHex(0x01b700);
                break;
            case 7:
                color = ofColor::fromHex(0x83ce01);
                break;
            case 8:
                color = ofColor::fromHex(0xfffa00);
                break;
            case 9:
                color = ofColor::fromHex(0xffcf00);
                break;
            case 10:
                color = ofColor::fromHex(0xffa600);
                break;
            case 11:
                color = ofColor::fromHex(0xff7d01);
                break;
        }
        spriteImg.load(filename);
        spriteImg.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    }
    
    void setAngularVelocity(float v) {
        body->SetAngularVelocity(v);
    }
    
    void checkToSendControl() {
        sendControl = false;
        tempo = (body->GetAngularVelocity()/75)*8;
        
        switch (senseType) {
            case (0): {
                if (tempo > 1) tempo = 1;
                if (tempo != prevTempo) {
                    sendControl = true;
                }
                break;
            }
            case (1): {
                if (x != prevX) sendControl = true;
                break;
            }
            case (2): {
                if (y != prevY) sendControl = true;
                break;
            }
            case (3): {
                if ((y != prevY) || (x!=prevX)) sendControl = true;
                break;
            }
        }
        prevTempo = tempo;
        prevX = x;
        prevY = y;
        if (abs(tempo) > 0.015) {
             spinning = true;
        } else {
             spinning = false;
        }
    }
    
    void drawAnimation(int stroke) {
        x = ofxBox2dBaseShape::getPosition().x;
        y = ofxBox2dBaseShape::getPosition().y;
        ofPushMatrix();
        ofTranslate(x, y);
        ofRotate(getRotation(), 0, 0, 1);
        if ((eyeState == 1) && (type < 144)) {
            if (count < maxAnimationCount) {
                count++;
            } else {
                count = 0;
            }
            ofNoFill();
            ofSetColor(color);
            ofSetLineWidth(stroke);
                for (int i=0; i < count; i++) {
                    ofDrawCircle(0, 0, displayW + i * animationStep);
                }
            ofFill();
        } else {
            count = 0;
        }
        
        ofPopMatrix();
    }
    
    void drawSoundWave(int stroke) {
        ofPushMatrix();
        ofTranslate(x, y);
        ofRotate(getRotation(), 0, 0, 1);
        ofSetLineWidth(stroke*retinaScale);
        if ((eyeState == 1) && (type<144)) {
            
            float x1 = soundWaveStart;
            ofSetColor(ofColor::fromHex(0x333333));
            for(int j = 0; j < scopeArray.size()-1; j++) {
                //ofDrawLine(x,scopeArray[j]*soundWaveH, x+soundWaveW,scopeArray[j+1]*soundWaveH);
                //ofDrawLine(x-soundWaveStart,scopeArray[j]*soundWaveH, x-soundWaveStart+soundWaveW,scopeArray[j+1]*soundWaveH);
                ofDrawLine(x1,scopeArray[j]*soundWaveH, x1+soundWaveStep,scopeArray[j+1]*soundWaveH);
                ofDrawLine(x1-soundWaveStart,scopeArray[j]*soundWaveH, x1-soundWaveStart+soundWaveStep,scopeArray[j+1]*soundWaveH);
                x1 += soundWaveStep;
            }
            ofFill();
        } else {
            count = 0;
        }
        ofPopMatrix();
    }
    
    void checkToSendNote() {
        if ((eyeState == 1) && (prevState == 0)) {
            sendOn = true;
            prevState = 1;
        }
        if ((eyeState == 0) && (prevState == 1 )) {
            sendOff = true;
            prevState = 0;
        }
    }
    
    Boolean inBounds(int x1, int y1) {
        // check id as well
        
        int x = ofxBox2dBaseShape::getPosition().x;
        int y = ofxBox2dBaseShape::getPosition().y;
        if ((x1 < (x+displayW/2)) &&
            (x1 > (x-displayW/2)) &&
            (y1 < (y+displayW/2)) &&
            (y1 > (y-displayW/2))) {
            return true;
        } else {
            return false;
        }
    }
    
    void draw() {
        if(body == NULL) {
            return;
        }
        x = ofxBox2dBaseShape::getPosition().x;
        y = ofxBox2dBaseShape::getPosition().y;
        rotation = getRotation();
        ofPushMatrix();
        ofSetColor(255, 255, 255);
        ofTranslate(x, y);
        ofRotate(rotation, 0, 0, 1);
        
        spriteImg.draw(0, 0, displayW, displayW);
        
        /* if (nJoints == 3) {
         ofSetHexColor(0xffffff);
         grayOverlay.draw(0, 0, w, w);
         }*/
        
        ofSetHexColor(0xFFFFFF);
        if (eyeState == 0) {
            //ofLog(OF_LOG_VERBOSE, "closed");
            myEyesClosed.draw(0, 0, displayW, displayW);
        } else {
            myEyesOpen.draw(0, 0, displayW, displayW);
        }
        // ofSetHexColor(0x000000);
        //vag.drawString(std::to_string(type), -5,-5);
        ofPopMatrix();
    }
    
    void drawBlueGlow() {
        ofPushMatrix();
        ofTranslate(x, y);
        blueGlow.draw(0, 0, displayW*2, displayW*2);
        ofPopMatrix();
    }
    
    void drawAsIcon(int x1, int y1, int size) {
        if(body == NULL) {
            return;
        }
        ofPushMatrix();
        //ofSetColor(color.r, color.g, color.b);
        ofSetColor(255, 255, 255);
        ofTranslate(x1, y1);

        spriteImg.draw(0, 0, size, size);
        myEyesOpen.draw(0, 0, size/2, size/2);
        ofPopMatrix();
    }
};

class FluxlyBubble : public ofxBox2dRect {
public:
    FluxlyBubble() {
    }
    
    int id;
    int x;
    int y;
    int w;
    int h;
    int a1x;
    int a1y;
    int a2x;
    int a2y;
    int a3x;
    int a3y;
    string bLabel;
    string bValue;
    int displayW;
    bool eyeState = false;
    bool onOffState = false;
    int nJoints = 0;
    int connections[4];
    int count = 0;
    //int stroke = 1;
    int prevState =0;
    float dampingX;
    float dampingY;
    bool touched = false;
    int touchId = -1;
    float rotation = 0.0;
    ofTrueTypeFont silkscreen;
    ofTrueTypeFont silkscreen2;
    int lineHeight;
    int fontSize = 1;
    
    b2BodyDef * def;
    
    float retinaScale;
    
    void init() {
        silkscreen.load("slkscr.ttf", 16);
        silkscreen2.load("slkscr.ttf", 10);
        lineHeight = silkscreen.getLineHeight()*.8;
    }
    
    void setAngularVelocity(float v) {
        body->SetAngularVelocity(v);
    }

    Boolean inBounds(int x1, int y1) {
        // check id as well
        
        int x = ofxBox2dBaseShape::getPosition().x;
        int y = ofxBox2dBaseShape::getPosition().y;
        if ((x1 < (x+displayW/2)) &&
            (x1 > (x-displayW/2)) &&
            (y1 < (y+displayW/2)) &&
            (y1 > (y-displayW/2))) {
            return true;
        } else {
            return false;
        }
    }
    
    void draw() {
        if(body == NULL) {
            return;
        }
        x = ofxBox2dBaseShape::getPosition().x;
        y = ofxBox2dBaseShape::getPosition().y;
        rotation = getRotation();
        ofPushMatrix();
        ofSetColor(255, 255, 255);
        ofTranslate(x, y);
        ofRotate(rotation, 0, 0, 1);
        
        ofSetColor(255);
        ofDrawRectRounded(0, 0, w, h, 10);
        ofSetColor(0);
        silkscreen.drawString(bLabel, -silkscreen.stringWidth(bLabel)/2, -3);
        if (fontSize == 0) {
            silkscreen2.drawString(bValue, -silkscreen2.stringWidth(bValue)/2, lineHeight-3);
        } else {
            silkscreen.drawString(bValue, -silkscreen.stringWidth(bValue)/2, lineHeight-3);
        }
        
        //ofDrawTriangle(bx+a1x, by+a1y, x+a3x, y+a3y, bx+a2x, by+a2y);
        
        /* if (nJoints == 3) {
         ofSetHexColor(0xffffff);
         grayOverlay.draw(0, 0, w, w);
         }*/
        ofPopMatrix();
    }
    
    void drawBubbleStem(int x1, int y1) {
          if(body == NULL) {
              return;
          }
        ofSetColor(255);
        ofSetLineWidth(5);
        ofDrawTriangle(x-5, y, x1, y1, x+5, y);
        ofDrawTriangle(x-5, y+5, x1, y1, x+5, y-5);
        //ofDrawLine(x, y, x1, y1);
    }
};

//--------------------------------------------
/*
class FluxlyIcon : public ofxBox2dCircle {
public:
    FluxlyIcon() {
    }
    
    int type;
    int id;
    int x;
    int y;
    int w;
    int eyePeriod;
    
    ofImage myEyesOpen;
    ofImage myEyesClosed;
    ofImage spriteImg;
    
    void init() {
        myEyesOpen.load("eyesOpen.png");
        myEyesClosed.load("eyesClosed.png");
        spriteImg.load("mesh" + std::to_string(type) + ".png");
    }
    
    
    Boolean inBounds(int x1, int y1) {
        // check id as well
        
        if ((x1 < (x+w/2)) &&
            (x1 > (x-w/2)) &&
            (y1 < (y+w/2)) &&
            (y1 > (y-w/2))) {
            return true;
        } else {
            return false;
        }
    }
    
    void draw() {
        ofPushMatrix();
        //ofSetColor(color.r, color.g, color.b);
        ofSetColor(255, 255, 255);
        ofTranslate(x, y);
        
        spriteImg.draw(0, 0, w, w);

        ofPopMatrix();
    }
};
*/

class FluxlyMenuItem : public ofxBox2dRect {
public:
    FluxlyMenuItem() {
    }
    
    int type = 0;
    int id;
    int x;
    int y;
    int w;
    int h;
    string filename;
    string link;
    ofImage spriteImg;
    ofImage myEyesOpen;
    ofImage myEyesClosed;
    bool eyeOpenState = false;
    float retinaScale;
    ofImage starMask;
    ofImage starMaskAlpha;
    float starWidth = 171;   // 684 on iPad
    float starHeight = 57.5;  // 230 on iPad
    float timeInScene = .5;
    bool drawStars = true;
    
    void init() {
        spriteImg.load(filename);
        spriteImg.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
        myEyesOpen.load("eyesOpen.png");
        myEyesClosed.load("eyesClosed.png");
        starMaskAlpha.load("starMaskAlpha.png");
       // starMaskAlpha.crop(0, 0, 684*timeInScene, 230);
        
       // starMask.load("starMask.png");
        
        
        //ofLog(OF_LOG_VERBOSE, "Load menuItem%d", id);
        //maxCount = ofRandom(10, 200);
        //color = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
    }
    
    void reloadThumbnail() {
       // ofLog(OF_LOG_VERBOSE, "Reload:"+filename);
        spriteImg.load(filename);
    }
    
    Boolean inBounds(int x1, int y1) {
        // check id as well
        //ofLog(OF_LOG_VERBOSE, "Checking: %d, %d vs %d, %d / %d, %d", x1, y1, (x),(y), (x+w),(y+h) );
        if ((x1 < (x+w)) &&
            (x1 > (x)) &&
            (y1 < (y+h)) &&
            (y1 > (y))) {
            return true;
        } else {
            return false;
        }
    }
    
    void drawWithOffset(int x1, int y1) {
        ofPushMatrix();
        //ofSetColor(color.r, color.g, color.b);
        ofSetColor(255, 255, 255);
        ofTranslate(x+x1, y+y1);
        
        int x2 = w/2-starWidth/2;
        int y2 = h/5;
       // ofLog(OF_LOG_VERBOSE, "sw sh: %f, %f, %f", starWidth, starHeight, starWidth*timeInScene);
        spriteImg.draw(0, 0, w, h);
        //if (drawStars) {
        //    starMaskAlpha.draw(x2, y2, starWidth*timeInScene, starHeight);
        //    starMask.draw(x2, y2, starWidth, starHeight);
        //}
        ofPopMatrix();
    }

  
    
    void drawBorderWithOffset(int x1, int y1) {
        ofSetHexColor(0xffcc00);
        ofSetLineWidth(3);
        ofNoFill();
        ofDrawRectangle(x+x1, y+y1, w, h);
        ofFill();
        ofSetColor(255, 255, 255);
    }
};
///////////////////////////////////////////////////

class SampleConsole {
public:
    SampleConsole() {
    }
    bool playing = false;
    bool recording = false;
    int x;
    int y;
    int w;
    int h;
    int playX;
    int playY;
    int recX;
    int recY;
    int playW = 60  ;
    int selected = -1;
    int thumbW = 80;

    float soundWaveStep = .5;
    float soundWaveH = 100;
    float soundWaveStart;

    ofImage myEyesOpen;
    ofImage myEyesClosed;
    ofImage sampleThumb;
    ofImage recordButton;
    ofImage playButton;
    ofImage stopRecButton;
    ofImage stopPlayButton;
    ofImage recordDisabled;
    
    vector<float> scopeArray;
    
    float retinaScale;
    
    void init(int w1, int h1) {
        w = w1;
        h = h1;
        playX = w/3;
        playY = 140 * retinaScale;
        recX = w-w/3;
        recY = 140 * retinaScale;
        thumbW *= retinaScale;
        playW *= retinaScale;
 
        sampleThumb.load("mesh0.png");
        recordButton.load("consoleRec.png");
        playButton.load("consolePlay.png");
        stopRecButton.load("consoleRecStop.png");
        stopPlayButton.load("consolePlayStop.png");
        recordDisabled.load("recordDisabled.png");
        myEyesOpen.load("eyesOpen.png");
        myEyesClosed.load("eyesClosed.png");
        //ofLog(OF_LOG_VERBOSE, "Load Sample Console");
       
    }
    
    void setSelected(int m) {
        selected = m;
        sampleThumb.load("mesh"+to_string(m)+".png");
    }
    
    int checkConsoleButtons(int x1, int y1) {
        int retval = 0;
        if ((x1 < (playX + playW/2)) && (x1 > (playX-playW/2)) &&
            (y1 < (playY + playW/2)) && (y1 > (playY - playW/2))) {
            ofLog(OF_LOG_VERBOSE, "Touched Play %d", playing);
            if (!recording) {
                playing = !playing;
                retval = 1;
            }
        }
#ifndef FLUXLY_FREE
        if (selected > 14) {
            if ((x1 < (recX + playW/2)) && (x1 > (recX - playW/2)) &&
                (y1 < (recY + playW/2)) && (y1 > (recY - playW/2))) {
                ofLog(OF_LOG_VERBOSE, "Touched Rec" );
                if (!playing) {
                    recording = !recording;
                    retval = 2;
                }
            }
        }
#endif
        return retval;
    }
    
    void drawSoundWave(int stroke) {
        // note that translate was done previous to call
        ofSetLineWidth(stroke);
        ofNoFill();
        soundWaveStep = (float)w/SCOPE_SIZE;
        soundWaveStart = -w/2;
        float x1 = soundWaveStart;
        ofSetColor(ofColor::fromHex(0xffffff));
       
        for(int j = 0; j < scopeArray.size()-1; j++) {
                ofDrawLine(x1,scopeArray[j]*soundWaveH, x1+soundWaveStep,scopeArray[j+1]*soundWaveH);
               //ofLog(OF_LOG_VERBOSE, "size %f" , x1);
                x1 += soundWaveStep;
         }
         ofFill();
    }
    
    void draw() {
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetColor(10, 10, 10, 220);
        ofDrawRectangle(0, 0, w, h);
        ofPushMatrix();
        ofSetColor(255, 255, 255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(w/2, 80*retinaScale);
        if (playing || recording) drawSoundWave(1);
        sampleThumb.draw(0, 0, thumbW, thumbW);
        myEyesOpen.draw(0, 0, thumbW, thumbW);
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(playX, playY);
        if (playing) {
            stopPlayButton.draw(0, 0, playW, playW);
        } else {
            playButton.draw(0, 0, playW, playW);
        }
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(recX, recY);
        
#ifndef FLUXLY_FREE
        if (recording) {
            stopRecButton.draw(0, 0, playW, playW);
        } else {
            if ((selected < 15) || playing) {
               recordDisabled.draw(0, 0, playW, playW);
            } else {
                recordButton.draw(0, 0, playW, playW);
            }
        }
#endif
#ifdef FLUXLY_FREE
        recordDisabled.draw(0, 0, playW, playW);
#endif
        ofPopMatrix();
    }
};

//--------------------------------------------

class SlidingMenu {
public:
    int type;
    int direction = 0;   // 1 = vertical / 0 = horizontal
    int nMenuItems;
    int nBanks;
    int menuItemsPerRow = 1;
    int menuItemW;
    int menuItemH;
    int menuOriginX = 0;
    int menuOriginY = 0;
    int consoleH = 200;
    int bankTitleW = 200;
    int bankTitleH = 10;
    float menuX = 0;
    float menuY = 0;
    int menuW;
    int menuH;
    int menuTitleW = 0;
    int menuTitleH = 0;
    int uniqueId;
    int maxPanes = 3;  // note: index, not number
    int currentPane = 1;
    float menuMoveStep = 0;
    int scrollingState = 0;
    int prevState = 0;
    int margin = 1;
    int selected = 0;
    int circleToChange = -1;
    
    float retinaScale = 1;
    float deviceScale = 1;
    
    int bankMargin = 0;
    
    ofImage title;
    ofImage bank0;
    ofImage bank1;
    
    string menuFilename;
    //string menuTitleFilename = "fluxlyTitle.png";
  
    vector <shared_ptr<FluxlyMenuItem> > menuItems;
    vector<float> scopeArray;
    
    void initMenu( int t, int x1, int y1, int w1, int h1) {
        type = t;
        menuW = w1;
        menuH = h1;
        menuX = x1;
        menuY = y1;
        consoleH = y1;
        menuOriginY = y1;
        menuOriginX = x1;
        
        ofxXmlSettings menuSettings;
        
        for (int i=0; i<256; i++) {
            scopeArray.push_back(0);
        }
        
        if (type == MAIN_MENU) menuFilename = ofxiOSGetDocumentsDirectory()+"menuSettings.xml";

        if (menuSettings.loadFile(menuFilename)) {
            uniqueId = menuSettings.getValue("settings:uniqueId", 0);
            //ofLog(OF_LOG_VERBOSE, "UniqueId %d:",uniqueId);
            menuSettings.pushTag("settings");
            menuSettings.pushTag("menuItems");
            nBanks = menuSettings.getNumTags("bank");
            for (int b=0; b<nBanks; b++) {
                menuSettings.pushTag("bank", b);
                int n = menuSettings.getNumTags("menuItem");
                nMenuItems += n;
                // ofLog(OF_LOG_VERBOSE, "menuItems %d:",nMenuItems);
                for (int i=0; i<n; i++) {
                    menuItems.push_back(shared_ptr<FluxlyMenuItem>(new FluxlyMenuItem));
                    FluxlyMenuItem * m = menuItems.back().get();
                    menuSettings.pushTag("menuItem", i);
                    m->id = menuSettings.getValue("id", 0);
                    //ofLog(OF_LOG_VERBOSE, "Id %d:",m->id);
                    if (type == MAIN_MENU)  m->filename = ofxiOSGetDocumentsDirectory()+menuSettings.getValue("img", "foo.png");
                    ofLog(OF_LOG_VERBOSE, m->filename);
                    m->link = menuSettings.getValue("link", "foo.xml");
                    if (m->link == "links.xml") {
                        m->drawStars = false;
                    } else {
                        m->drawStars = true;
                    }
                    m->starWidth *= retinaScale * deviceScale;
                    m->starHeight *= retinaScale * deviceScale;
                    
                    //ofLog(OF_LOG_VERBOSE, m->link);
                    m->timeInScene = menuSettings.getValue("timeInScene", 0);
                    if (m->timeInScene > 900) m->timeInScene = 900;
                    m->timeInScene = (m->timeInScene - 0) * (.9 - .1) / (900 - 0) + .1;
                    if (m->timeInScene > 1) m->timeInScene = 1;
                    if (m->timeInScene < .1) m->timeInScene = .1;
                    ofLog(OF_LOG_VERBOSE, "time in Scene %f", m->timeInScene);
                    m->init();
                    menuSettings.popTag();
                }
                menuSettings.popTag();
            }
        }
        
         //title.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
        //Add menuItems
        
        //int margin = (menuW-menuItemW*menuItemsPerRow)/(menuItemsPerRow+1);
        margin = 1;
        
        if (type == MAIN_MENU) {
            menuItemsPerRow = 1;
            menuItemW = menuW/menuItemsPerRow-margin;
            menuItemH = menuH*((float)menuItemW/menuW);
            //float ratio =((float)menuItemW/menuW);
            //maxPanes = ceil(nMenuItems/menuItemsPerRow);
            maxPanes = 5;  // hardcoded for now
            //ofLog(OF_LOG_VERBOSE, "maxPanes %i", maxPanes);
        }
        
        int rows = nMenuItems/menuItemsPerRow+1;
        
        for (int row=0; row < rows; row++) {
            for (int col=0; col < menuItemsPerRow; col++) {
                int index = col+row*menuItemsPerRow;
                if (index < nMenuItems) {
                  menuItems[index]->type = type;
                  menuItems[index]->w = menuItemW;
                  menuItems[index]->h = menuItemH;
                  menuItems[index]->x = margin+col*(menuItemW + margin);
                  menuItems[index]->y = margin+row * (menuItemH+margin);
                  //ofLog(OF_LOG_VERBOSE, "bankMargin, y , scale %d %d %f",bankMargin,menuItems[index]->y, retinaScale );
                    
                  //ofLog(OF_LOG_VERBOSE, "Menu Item X, Y %d, %d:",menuItems[col+row*menuItemsPerRow]->x, menuItems[col+row*menuItemsPerRow]->y);
                }
            }
        }
    }
    
    void changePaneState(int dir) {
        int newPaneState = currentPane - dir;
        ofLog(OF_LOG_VERBOSE, "New pane state: %i", newPaneState);
        if (type == MAIN_MENU) {
            if ((newPaneState > 0) && (newPaneState < maxPanes)) {
                if (newPaneState == 1) {
                    menuOriginX = 0;
                    menuOriginY = 0;
                }
                if (newPaneState == 2) {
                    menuOriginX = 0;
                    menuOriginY = - menuItemH - 3 * margin;
                }
                if (newPaneState > 2) {
                    menuOriginX = 0;
                    menuOriginY = - menuItemH * (newPaneState-1) - 3 * margin ;
                }
                currentPane = newPaneState;
            }
        }
    }
    
    void drawBorder(int index) {
       // if (index >=0) {
       //    menuItems[index]->drawBorderWithOffset(menuX, menuY);
       // }
    }
    
    void updateScrolling() {
        if (type == MAIN_MENU) {
            // vertical scroll
            if ((scrollingState == 2) && (menuY < menuOriginY)) {
                menuY+=menuMoveStep;
                //ofLog(OF_LOG_VERBOSE, "move to origin - %d, %f:",menuY,menuMoveStep);
            }
            if ((scrollingState == 2) && (menuY > menuOriginY)) {
                menuY-=menuMoveStep;
                //ofLog(OF_LOG_VERBOSE, "move to origin + %d, %f:",menuY,menuMoveStep);
            }
            if ((scrollingState == 2) && (abs(menuY-menuOriginY) < menuMoveStep)) {
                menuY = menuOriginY;
                scrollingState = 0;
            }
        }
        if (type == SAMPLE_MENU) {
            // vertical scroll
            if ((scrollingState == 2) && (menuY < menuOriginY)) {
                menuY+=menuMoveStep;
                //ofLog(OF_LOG_VERBOSE, "move to origin - %d, %f:",menuY,menuMoveStep);
            }
            if ((scrollingState == 2) && (menuY > menuOriginY)) {
                menuY-=menuMoveStep;
                //ofLog(OF_LOG_VERBOSE, "move to origin + %d, %f:",menuY,menuMoveStep);
            }
            if ((scrollingState == 2) && (abs(menuY-menuOriginY) < menuMoveStep)) {
                menuY = menuOriginY;
                scrollingState = 0;
            }
        }
    }
    
    int checkMenuTouch( int x1, int y1) {
        int retval = -1;
        if ((type == MAIN_MENU) || ((type == SAMPLE_MENU) && (y1 > consoleH))) {
            for (int i=0; i<nMenuItems; i++) {
                if (menuItems[i].get()->inBounds(x1-menuX, y1-menuY)) {
                    ofLog(OF_LOG_VERBOSE, "Touched %d", menuItems[i].get()->id);
                    retval = menuItems[i].get()->id;
                }
            }
        }
        return retval;
    }
    
    void updateEyeState() {
        for (int i=0; i<nMenuItems; i++) {
            if (i == selected) {
                menuItems[i].get()->eyeOpenState = true;
            } else {
                menuItems[i].get()->eyeOpenState = false;
            }
        }
    }
    void draw() {
        ofSetRectMode(OF_RECTMODE_CORNER);
        if (type == MAIN_MENU) {
            ofBackground(0, 0, 0);
            //title.draw(0, 0, menuTitleW, menuTitleH+menuY);
        }
        for (int i=0; i < nMenuItems; i++) {
             drawVisualization(i, menuX, menuY);
            menuItems[i].get()->drawWithOffset(menuX, menuY);
        }
    }
    
    void drawVisualization(int n, int xOffset, int yOffset) {
        ofPushMatrix();
        ofTranslate(menuItems[n].get()->x, menuItems[n].get()->x+yOffset + (menuItems[n].get()->h *n));

        switch (n) {
            case 0: {
                int w = menuItems[n].get()->h/16;
                ofSetLineWidth(2*retinaScale);
                for (int i=0; i<256; i++) {
                    int t = (int)(scopeArray[i] * 255);
                    int w1 =w*1.5;
                    for (int j=0; j<8; j++) {
                        if (((t >> j) & 0x01) == 1) {
                            ofSetColor(255, 255, 255, 127);
                            // if (circles[0]->tempo == 0) ofSetColor(0);
                            //ofNoFill();
                            ofDrawRectangle(i*w, menuItems[n].get()->h/2-j*(w)-w/2, w1, w1);
                            ofDrawRectangle(i*w, menuItems[n].get()->h/2+j*(w)-w/2, w1, w1);
                            ofFill();
                        } else {
                            ofSetColor(0);
                            //dw = 10; dh=10;
                        }
                    }
                }
                break;
            }
            case 1: {
                float x1 = 0;
                float h =menuItems[n].get()->h/2;
                float w =menuItems[n].get()->w;
                float step = (menuItems[n].get()->w*retinaScale)/scopeArray.size()/(2*retinaScale);
                ofSetLineWidth(1);
                ofSetColor(ofColor::fromHex(0xffffff));
                
                for (int j = 0; j < scopeArray.size()-1; j++) {
                    ofDrawLine(0,0, x1+step, 2*scopeArray[j+1]*h+h);
                    ofDrawLine(w, 0, w-x1-step, 2*scopeArray[j+1]*h+h);
                    ofDrawLine(x1,2*scopeArray[j]*h+h, x1+step, 2*scopeArray[j+1]*h+h);
                    ofDrawLine(w-x1,2*scopeArray[j]*h+h, w-x1-step, 2*scopeArray[j+1]*h+h);
                    x1 += step;
                }
                ofFill();
                break;
            }
            case 2: {
                int w1 = menuItems[n].get()->h/4;
                float w2 = w1/4;
                int xc = menuItems[n].get()->w/2;
                int yc = menuItems[n].get()->h/2;
                ofPushMatrix();
                ofSetLineWidth(2*retinaScale);
                ofSetColor(ofColor::fromHex(0xffffff));
                ofFill();
                ofTranslate(xc, yc + w2/2);
                for (int col = 1; col < 4; col++) {
                    for (int row = 1; row < 3; row++) {
                        for (int i = 0; i < 4; i++) {
                            for (int j=0; j<4; j++) {
                                if ((((int)(scopeArray[0]*65536) >> (i * j + j)) & 0x01) == 1) {
                    
                                        ofDrawEllipse(j*w2-(col*w1), i*w2-(row*w1), w2*.75, w2*.75);
                                        ofDrawEllipse(j*w2-(col*w1) + w1*3, i*w2-(row*w1), w2*.75, w2*.75);
                                        ofDrawEllipse(j*w2-(col*w1), i*w2-(row*w1)+w1*2, w2*.75, w2*.75);
                                        ofDrawEllipse(j*w2-(col*w1)+w1*3, i*w2-(row*w1)+w1*2, w2*.75, w2*.75);
                                }
                            }
                        }
                    }
                }
                ofPopMatrix();
                break;
            }
        }
        
        ofPopMatrix();
    }
};



#endif /* customClasses_h */
