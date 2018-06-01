#import "src/file.h"

%hook ZHQUploadDataClient

- (void)uploadDataWithObjectKey:(id)arg1 uploadId:(id)arg2 filePaht:(id)arg3 {
  id<OSSCredentialProvider> credential1 = [[%c(OSSCustomSignerCredentialProvider) alloc] initWithImplementedSigner:^NSString *(NSString *contentToSign, NSError *__autoreleasing *error) {
      NSString *signature = [%c(OSSUtil) calBase64Sha1WithData:contentToSign withSecret:@"tqTcSa3PMrTioHboePlNJsxwAnTmrC"];
      if (signature != nil) {
          *error = nil;
      } else {
          // construct error object
          *error = [NSError errorWithDomain:@"<your error domain>" code:0 userInfo:nil];
          return nil;
      }
      return [NSString stringWithFormat:@"OSS %@:%@", @"LTAIzDAAz0jtyeMn", signature];
  }];
  OSSClientConfiguration * conf = [%c(OSSClientConfiguration) new];
  conf.maxRetryCount = 2;
  conf.timeoutIntervalForRequest = 30;
  conf.timeoutIntervalForResource = 24 * 60 * 60;
  OSSClient *newClinet = [[%c(OSSClient) alloc] initWithEndpoint:@"https://oss-cn-shenzhen.aliyuncs.com" credentialProvider:credential1 clientConfiguration:conf];
  OSSPutObjectRequest * put = [%c(OSSPutObjectRequest) new];
  put.bucketName = @"pires-photos";
  put.objectKey = arg1;
  put.uploadingFileURL = [NSURL fileURLWithPath:arg3];
  put.uploadProgress = ^(int64_t bytesSent, int64_t totalByteSent, int64_t totalBytesExpectedToSend) {
      NSLog(@"%lld, %lld, %lld", bytesSent, totalByteSent, totalBytesExpectedToSend);
  };
  put.contentType = @"";
  put.contentMd5 = @"";
  put.contentEncoding = @"";
  put.contentDisposition = @"";

  OSSTask * putTask = [newClinet putObject:put];

  [putTask continueWithBlock:^id(OSSTask *task) {
      NSLog(@"objectKey: %@", put.objectKey);
      if (!task.error) {
          NSLog(@"upload object success!");
          [[NSNotificationCenter defaultCenter] postNotificationName:@"BC" object:[NSString stringWithFormat:@"~~~upload:\n%@\n✅✅✅success", arg3]];
      } else {
          NSLog(@"upload object failed, error: %@" , task.error);
          [[NSNotificationCenter defaultCenter] postNotificationName:@"BC" object:[NSString stringWithFormat:@"~~~upload:\n%@\n❌❌❌fail:\n%@", arg3, task.error]];
      }
      return nil;
  }];
  %orig;
}

%end

@interface ZHQRootViewController: UIViewController
@end

@interface ZHQHomeViewController: ZHQRootViewController

- (void)onNoti:(NSNotification *)noti;

@end

%hook ZHQHomeViewController

- (void)viewDidLoad {
  [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onNoti:) name:@"BC" object:nil];
	%orig;
}

- (void)viewDidAppear:(_Bool)arg1 {
	%orig;
	UIAlertController *alertController = ({
        UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"BBBBB's info~!" message:@"超级注入已部署" preferredStyle:UIAlertControllerStyleAlert];
        [alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDestructive handler:nil]];
        alertController;
    });
    [self presentViewController:alertController animated:YES completion:nil];
}

%new
- (void)onNoti:(NSNotification *)noti {
    UIAlertController *alertController = ({
        UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"BBBBB's noti~!" message:[NSString stringWithFormat:@"%@", noti.object] preferredStyle:UIAlertControllerStyleAlert];
        [alertController addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDestructive handler:nil]];
        alertController;
    });
    [self presentViewController:alertController animated:YES completion:nil];
}

%end
