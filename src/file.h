
typedef NSString * (^OSSCustomSignContentBlock) (NSString * contentToSign, NSError **error);
typedef void (^OSSNetworkingUploadProgressBlock) (int64_t bytesSent, int64_t totalBytesSent, int64_t totalBytesExpectedToSend);

@interface OSSUtil : NSObject
+ (id)base64ForData:(char *)arg1 length:(int)arg2;
+ (id)base64Md5ForData:(id)arg1;
+ (id)base64Md5ForFilePath:(id)arg1;
+ (id)base64Md5ForFileURL:(id)arg1;
+ (id)calBase64Sha1WithData:(id)arg1 withSecret:(id)arg2;
+ (id)calBase64WithData:(char *)arg1;
+ (id)constructHttpBodyForCreateBucketWithLocation:(id)arg1;
+ (id)constructHttpBodyFromPartInfos:(id)arg1;
+ (id)convertMd5Bytes2String:(char *)arg1;
+ (id)dataMD5:(id)arg1;
+ (id)dataMD5String:(id)arg1;
+ (id)detemineMimeTypeForFilePath:(id)arg1 uploadName:(id)arg2;
+ (id)encodeURL:(id)arg1;
+ (id)fileMD5:(id)arg1;
+ (id)fileMD5String:(id)arg1;
+ (id)getIpByHost:(id)arg1;
+ (id)getRelativePath:(id)arg1;
+ (_Bool)isNetworkDelegateState;
+ (_Bool)isOssOriginBucketHost:(id)arg1;
+ (id)sign:(id)arg1 withToken:(id)arg2;
+ (_Bool)validateBucketName:(id)arg1;
+ (_Bool)validateObjectKey:(id)arg1;
@end

@protocol OSSCredentialProvider <NSObject>
@optional
- (NSString *)sign:(NSString *)content error:(NSError **)error;
@end

@interface OSSCustomSignerCredentialProvider : NSObject <OSSCredentialProvider>
@property (nonatomic, copy) NSString * (^signContent)(NSString *, NSError **);
- (instancetype)initWithImplementedSigner:(OSSCustomSignContentBlock)signContent;
@end

@interface OSSClientConfiguration : NSObject

/**
 最大重试次数
 */
@property (nonatomic, assign) uint32_t maxRetryCount;

/**
 最大并发请求数
 */
@property (nonatomic, assign) uint32_t maxConcurrentRequestCount;

/**
 是否开启后台传输服务
 注意：只在上传文件时有效
 */
@property (nonatomic, assign) BOOL enableBackgroundTransmitService;

/**
 是否使用Httpdns解析域名
 */
@property (nonatomic, assign) BOOL isHttpdnsEnable;

/**
 设置后台传输服务使用session的Id
 */
@property (nonatomic, strong) NSString * backgroundSesseionIdentifier;

/**
 请求超时时间
 */
@property (nonatomic, assign) NSTimeInterval timeoutIntervalForRequest;

/**
 单个Object下载的最长持续时间
 */
@property (nonatomic, assign) NSTimeInterval timeoutIntervalForResource;

/**
 设置代理Host、端口
 */
@property (nonatomic, strong) NSString * proxyHost;
@property (nonatomic, strong) NSNumber * proxyPort;

/**
 设置Cname排除列表
 */
@property (nonatomic, strong, setter=setCnameExcludeList:) NSArray * cnameExcludeList;

@end


@interface OSSRequest : NSObject
/**
 指明该请求是否需要鉴权，单次有效
 */
@property (nonatomic, assign) BOOL isAuthenticationRequired;

/**
 指明该请求是否已经被取消
 */
@property (nonatomic, assign) BOOL isCancelled;

/**
 取消这个请求
 */
- (void)cancel;
@end

/**
 请求结果的基类
 */
@interface OSSResult : NSObject

/**
 请求HTTP响应码
 */
@property (nonatomic, assign) NSInteger httpResponseCode;

/**
 请求HTTP响应头部，以KV形式放在字典中
 */
@property (nonatomic, strong) NSDictionary * httpResponseHeaderFields;

/**
 x-oss-request-id是由Aliyun OSS创建，并唯一标识这个response的UUID。如果在使用OSS服务时遇到问题，可以凭借该字段联系OSS工作人员，快速定位问题。
 */
@property (nonatomic, strong) NSString * requestId;
@end

@interface OSSPutObjectRequest : OSSRequest

/**
 Bucket名称
 */
@property (nonatomic, strong) NSString * bucketName;

/**
 Object名称
 */
@property (nonatomic, strong) NSString * objectKey;

/**
 从内存中的NSData上传时，通过这个字段设置
 */
@property (nonatomic, strong) NSData * uploadingData;

/**
 从文件上传时，通过这个字段设置
 */
@property (nonatomic, strong) NSURL * uploadingFileURL;

/**
 server回调参数设置
 */
@property (nonatomic, strong) NSDictionary * callbackParam;

/**
 server回调变量设置
 */
@property (nonatomic, strong) NSDictionary * callbackVar;

/**
 设置文件类型
 */
@property (nonatomic, strong) NSString * contentType;

/**
 根据协议RFC 1864对消息内容（不包括头部）计算MD5值获得128比特位数字，对该数字进行base64编码为一个消息的Content-MD5值。
 该请求头可用于消息合法性的检查（消息内容是否与发送时一致）。虽然该请求头是可选项，OSS建议用户使用该请求头进行端到端检查。
 */
@property (nonatomic, strong) NSString * contentMd5;

/**
 指定该Object被下载时的名称；更详细描述请参照RFC2616。
 */
@property (nonatomic, strong) NSString * contentDisposition;

/**
 指定该Object被下载时的内容编码格式；更详细描述请参照RFC2616。
 */
@property (nonatomic, strong) NSString * contentEncoding;

/**
 指定该Object被下载时的网页的缓存行为；更详细描述请参照RFC2616。
 */
@property (nonatomic, strong) NSString * cacheControl;

/**
 过期时间（milliseconds）；更详细描述请参照RFC2616。
 */
@property (nonatomic, strong) NSString * expires;

/**
 可以在这个字段中携带以x-oss-meta-为前缀的参数，则视为user meta，比如x-oss-meta-location。一个Object可以有多个类似的参数，但所有的user meta总大小不能超过8k。
 如果上传时还需要指定其他HTTP请求头字段，也可以在这里设置
 */
@property (nonatomic, strong) NSDictionary * objectMeta;

/**
 上传进度回调，
 会在任务执行的后台线程被回调，而非UI线程
 */
@property (nonatomic, copy) OSSNetworkingUploadProgressBlock uploadProgress;
@end


@interface OSSTask : NSObject

- (id)continueWithBlock:(id)arg1;
@property(readonly, nonatomic) NSError *error;

@end

@interface OSSClient : NSObject
- (id)initWithEndpoint:(id)arg1 credentialProvider:(id)arg2 clientConfiguration:(id)arg3;
- (OSSTask *)putObject:(OSSPutObjectRequest *)request;

@end


