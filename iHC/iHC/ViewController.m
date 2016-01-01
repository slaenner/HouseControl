//
//  ViewController.m
//  iHC
//
//  Created by Stefan Lænner on 26/08/15.
//  Copyright © 2015 Stefan Lundal Lænner. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize lightControlView;
@synthesize networkStreamIn, networkStreamOut;
@synthesize cmdTextField;
@synthesize messages;
@synthesize cmdLabel;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self initNetworkCommunication];
    messages = [[NSMutableArray alloc] init];
    NSString *response  = [NSString stringWithFormat:@"iam:Controller"];
    NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    [networkStreamOut write:[data bytes] maxLength:[data length]];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void) initNetworkCommunication
{
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)@"192.168.1.18", 9930, &readStream, &writeStream);
    
    networkStreamIn = (__bridge NSInputStream *)readStream;
    networkStreamOut = (__bridge NSOutputStream *)writeStream;
    [networkStreamIn setDelegate:self];
    [networkStreamOut setDelegate:self];
    [networkStreamIn scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [networkStreamOut scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [networkStreamIn open];
    [networkStreamOut open];
    cmdLabel.text = @"Hej";
}

- (IBAction)sendSegSelMessage:(id)sender
{
    NSString *str = @"";
    NSArray *strArray = [NSArray arrayWithObjects:@"On", @"Off", nil];

    // Convert switch position into string
    str = strArray[self.segmentLightSelection.selectedSegmentIndex];
    
    NSString *response  = [NSString stringWithFormat:@"msg:%@", str];
    NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    [networkStreamOut write:[data bytes] maxLength:[data length]];
}

- (IBAction)sendCmdTextMessage:(id)sender
{
    NSString *response  = [NSString stringWithFormat:@"msg:%@", cmdTextField.text];
    NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    [networkStreamOut write:[data bytes] maxLength:[data length]];
    cmdTextField.text = @"";
}


- (void)stream:(NSStream *)theStream handleEvent:(NSStreamEvent)streamEvent
{
    NSLog(@"stream event %lu", (unsigned long)streamEvent);
    switch (streamEvent)
    {
        case NSStreamEventOpenCompleted:
            NSLog(@"Stream opened");
            break;

        case NSStreamEventHasBytesAvailable:
            if (theStream == networkStreamIn)
            {
                uint8_t buffer[1024];
                NSInteger len;
                
                while ([networkStreamIn hasBytesAvailable])
                {
                    len = [networkStreamIn read:buffer maxLength:sizeof(buffer)];
                    if (len > 0)
                    {
                        NSString *output = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
                        if (nil != output)
                        {
                            NSLog(@"server said: %@", output);
                            [self messageReceived:output];
                        }
                    }
                }
            }
            break;
            
            
        case NSStreamEventErrorOccurred:
            NSLog(@"Can not connect to the host!");
            break;
            
        case NSStreamEventEndEncountered:
            [theStream close];
            [theStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            theStream = nil;
            break;

        default:
            NSLog(@"Unknown event");
    }
    
}

- (void) messageReceived:(NSString *)message
{
    
    [self.messages addObject:message];
    
    if ([message rangeOfString:@"state = "].location == NSNotFound)
    {
        NSLog(@"string does not contains state = ");
    }
    else
    {
        NSLog(@"string contains state = !");
    }
    
    NSUInteger startIndex = [message rangeOfString:@"state = " options:NSBackwardsSearch].location + 8;
    NSUInteger length = [message length] - startIndex - 1;
    NSString *state = [message substringWithRange:NSMakeRange(startIndex, length)];
    
    if ([state isEqualToString:@"On"])
    {
        self.segmentLightSelection.selectedSegmentIndex = 0;
        cmdLabel.text = @"Server confirmed: On";
    }
    
    if ([state isEqualToString:@"Off"])
    {
        self.segmentLightSelection.selectedSegmentIndex = 1;
        cmdLabel.text = @"Server confirmed: Off";
    }
}

@end
