#import "ViewController.h"
#include "game.h"
#include "GameCore.h"

GameCore* pGameCore = new GameCore;
NSTimeInterval executionTime;
CGPoint touchBeganPoint;
float prevDistBetweenTwoPoints;


@interface ViewController () {
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
  
    
    pGameCore->OneTimeInit();
    
    //getting resolution
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    CGSize screenSize = CGSizeMake(screenBounds.size.width * screenScale, screenBounds.size.height * screenScale);
    
    pGameCore->OnSurfaceChanged( screenSize.width, screenSize.height );
    
}

-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    UITouch *touch = [[event allTouches] anyObject];
    touchBeganPoint = [touch locationInView:touch.view];
    
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    CGSize screenSize = CGSizeMake(screenBounds.size.width * screenScale, screenBounds.size.height * screenScale);
    
    pGameCore->OnSurfaceChanged( screenSize.height, screenSize.width ); //switching around device width and height
 
    NSLog(@"My screen width is %f ", screenSize.width);
    NSLog(@"My screen height is %f ", screenSize.height);
    
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
   //do somthing after rotation
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if ([[event allTouches] count] > 1)
    {
        UITouch *touch1, *touch2;
        CGPoint    location1, location2;
        
       // NSLog(@"%d active touches",[[event allTouches] count]) ;
        NSSet *allTouches = [event allTouches];
        touch1 = [[allTouches allObjects] objectAtIndex:0];
        location1 = [touch1 locationInView:touch1.view];
        
        touch2 = [[allTouches allObjects] objectAtIndex:1];
        location2 = [touch2 locationInView:touch2.view];
        
        float diffX = location2.x - location1.x;
        float diffY = location2.y - location1.y;
        float distance = sqrt( (diffX * diffX) + (diffY * diffY) );
        
        if (prevDistBetweenTwoPoints == 0)
            prevDistBetweenTwoPoints = distance;
        
        if(distance > prevDistBetweenTwoPoints)
            pGameCore->ZoomZoom(-0.1f);
        else if (distance < prevDistBetweenTwoPoints)
              pGameCore->ZoomZoom(0.1f);
        
        prevDistBetweenTwoPoints = distance;

        touchBeganPoint.x = 0;
        touchBeganPoint.y = 0;
    }
    else if ([[event allTouches] count] == 1)
    {
        
        UITouch *touch = [[event allTouches] anyObject];
        CGPoint location = [touch locationInView:touch.view];
        
        if(touchBeganPoint.x == 0 && touchBeganPoint.y == 0) //this is to prevent sudden movement when after zoom touch began location might become wrong value
            touchBeganPoint = location;
        float x = location.x - touchBeganPoint.x;
        float y = location.y - touchBeganPoint.y;
       // NSLog(@"My location x is %f \n",location.x);
        
        float distanceX = sqrt(x*x);
        float distanceY = sqrt(y*y);
        
        ////////////this code is fore view where we rotate just around y axis//////////
        if(x >= 0)
        pGameCore->RotateCameraAroundAxis(-distanceX);
        else
        pGameCore->RotateCameraAroundAxis(distanceX);
        ///////////////////////////////////////////////////////////////////////////////
        
        distanceX /= 100.0f; //deviding by 100 simply to slowdown user input rotation
        distanceY /= 100.0f;
    
        if(x >= 0)
        pGameCore->m_yAxisAngle += distanceX; //when we slide horizontally we expect object to rotate arount its x axis
        else
        pGameCore->m_yAxisAngle -= distanceX;
    
        if(y >= 0)
        pGameCore->m_xAxisAngle += distanceY;
        else
        pGameCore->m_xAxisAngle -= distanceY;
        
       // NSLog(@"distance is %f ", distanceX+distanceY);
         touchBeganPoint = location;

    }
    
   
    
   // NSLog(@"My distance is %f \n",distance);
   // NSLog(@"My distance2 is %f \n",distance2);
    
    
   }

#pragma mark - GLKView and GLKViewController delegate methods
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    NSDate *methodStart = [NSDate date];
    
    /* ... Do whatever you need to do ... */
    
   
   // NSLog(@"executionTime = %f", executionTime);
    //on_draw_frame();
   
    // do stuff...
   
   // NSLog(@"My number is %d haha", timeInterval);
    pGameCore->OnDrawFrame();
    pGameCore->Tick(executionTime*10);
    
    NSDate *methodFinish = [NSDate date];
    executionTime = [methodFinish timeIntervalSinceDate:methodStart];
}

- (IBAction)changeView:(id)sender
{
    pGameCore->SwitchView();
}

@end
