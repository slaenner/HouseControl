//
//  ViewController.h
//  iHC
//
//  Created by Stefan Lænner on 26/08/15.
//  Copyright © 2015 Stefan Lundal Lænner. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <NSStreamDelegate>
{
    UIView			*lightControlView;
    NSInputStream	*networkStreamIn;
    NSOutputStream	*networkStreamOut;
    UITextField		*cmdTextField;
    NSMutableArray	*messages;
    
}

@property (nonatomic, retain) NSInputStream *networkStreamIn;
@property (nonatomic, retain) NSOutputStream *networkStreamOut;
@property (nonatomic, retain) NSMutableArray *messages;
@property (nonatomic, retain) UIView *lightControlView;

// Outlets
@property (nonatomic, retain) IBOutlet UITextField *cmdTextField;
@property (nonatomic, retain) IBOutlet UIButton *buttonSend;
@property (nonatomic, retain) IBOutlet UISegmentedControl *segmentLightSelection;
@property (nonatomic, retain) IBOutlet UILabel *cmdLabel;

// Function prototypes
- (void) initNetworkCommunication;

// Action functions
- (IBAction)sendSegSelMessage:(id)sender;
- (IBAction)sendCmdTextMessage:(id)sender;


@end
