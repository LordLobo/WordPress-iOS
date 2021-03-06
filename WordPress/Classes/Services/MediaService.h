#import <Foundation/Foundation.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <Photos/Photos.h>
#import "LocalCoreDataService.h"

@class Media;
@class Blog;

@interface MediaService : LocalCoreDataService

/**
 Create a media object using the url provided as the source of media.

 @param url a file url pointing to a file with the media data
 @param postObjectID the post object ID to associate the media
 @param thumbnailCallback a block that will be invoked when the thumbail for the media object is ready
 @param completion a block that will be invoked when the media is created, on success it will return a valid Media object, on failure it will return a nil Media and an error object with the details.
 */
- (void)createMediaWithURL:(nonnull NSURL *)url
           forPostObjectID:(nonnull NSManagedObjectID *)postObjectID
         thumbnailCallback:(nullable void (^)(NSURL * _Nonnull thumbnailURL))thumbnailCallback
                completion:(nullable void (^)(Media * _Nullable media, NSError * _Nullable error))completion NS_SWIFT_NAME(createMedia(url:forPost:thumbnailCallback:completion:));

/**
 Create a Media object using the asset as the source and making it a child of the post with postObjectId.
 
 @param asset
 @param postObjectID
 @param thumbnailCallback a block that will be invoked when the thumbail for the media object is ready
 @param completion a block that will be invoked when the media is created, on success it will return a valid Media object, on failure it will return a nil Media and an error object with the details.
 */
- (void)createMediaWithPHAsset:(nonnull PHAsset *)asset
             forPostObjectID:(nonnull NSManagedObjectID *)postObjectID
           thumbnailCallback:(nullable void (^)(NSURL * _Nonnull thumbnailURL))thumbnailCallback
                    completion:(nullable void (^)(Media * _Nullable media, NSError * _Nullable error))completion;

- (void)createMediaWithImage:(nonnull UIImage *)image
                 withMediaID:(nonnull NSString *)mediaID
             forPostObjectID:(nonnull NSManagedObjectID *)postObjectID
           thumbnailCallback:(nullable void (^)(NSURL * _Nonnull thumbnailURL))thumbnailCallback
                  completion:(nullable void (^)(Media * _Nullable media, NSError * _Nullable error))completion;

- (void)createMediaWithPHAsset:(nonnull PHAsset *)asset
               forBlogObjectID:(nonnull NSManagedObjectID *)blogObjectID
             thumbnailCallback:(nullable void (^)(NSURL * _Nonnull thumbnailURL))thumbnailCallback
                    completion:(nullable void (^)(Media * _Nullable media, NSError * _Nullable error))completion;

- (void)createMediaWithImage:(nonnull UIImage *)image
             forBlogObjectID:(nonnull NSManagedObjectID *)blogObjectID
           thumbnailCallback:(nullable void (^)(NSURL * _Nonnull thumbnailURL))thumbnailCallback
                  completion:(nullable void (^)(Media * _Nullable media, NSError * _Nullable error))completion;

/**
 Get the Media object from the server using the blog and the mediaID as the identifier of the resource
 
 @param mediaID
 @param blog
 @param success a block that will be invoked when the media is retrieved
 @param failure a block that will be invoked if an error happens returnin the associated error object with the details.
 */
- (void)getMediaWithID:(nonnull NSNumber *)mediaID
                inBlog:(nonnull Blog *)blog
           withSuccess:(nullable void (^)(Media * _Nonnull media))success
               failure:(nullable void (^)(NSError * _Nonnull error))failure;

/**
 Uploads the Media object to the server.
 
 @param media object to upload to the server.
 @param progress a NSProgress that tracks the upload progress to the server.
 @param success a block that will be invoked when the media upload finished with success
 @param failure a block that will be invoked when there is upload error.
 */
- (void)uploadMedia:(nonnull Media *)media
           progress:(NSProgress * __nullable __autoreleasing * __nullable) progress
            success:(nullable void (^)())success
            failure:(nullable void (^)(NSError * _Nonnull error))failure;


/**
 Updates the media object details to the server. This method doesn't allow you to update media file,
 because that always stays static after the initial upload, it only allows to change details like,
 caption, alternative text, etc...

 @param media object to upload to the server.
 @success a block that will be invoked when the media upload finished with success
 @failure a block that will be invoked when there is upload error.
 */
- (void)updateMedia:(nonnull Media *)media
            success:(nullable void (^)())success
            failure:(nullable void (^)(NSError * _Nonnull error))failure;

/**
 Updates multiple media objects similar to -updateMedia:success:failure: but batches them
 together. The success handler is only called when all updates succeed. Failure is called
 if the entire process fails in some catostrophic way.
 
 @param mediaObjects An array of media objects to update
 @param success
 */
- (void)updateMedia:(nonnull NSArray<Media *> *)mediaObjects
     overallSuccess:(nullable void (^)())overallSuccess
            failure:(nullable void (^)(NSError * _Nonnull error))failure;

/**
 Deletes the Media object from the server. Note the Media is deleted, not trashed.

 @param media object to delete.
 @param success a block that will be invoked when the media deletion finished with success
 @param failure a block that will be invoked when there is an error.
 */
- (void)deleteMedia:(nonnull Media *)media
            success:(nullable void (^)())success
            failure:(nullable void (^)(NSError * _Nonnull error))failure;

/**
 Deletes multiple Media objects from the server. Note the Media objects are deleted, not trashed.

 @param mediaObjects An array of media objects to delete.
 @param progress a block that will be invoked after each media item is deleted successfully
 @param success a block that will be invoked when the media deletion finished with success
 @param failure a block that will be invoked when there is an error.
 */
- (void)deleteMedia:(nonnull NSArray<Media *> *)mediaObjects
           progress:(nullable void (^)(NSProgress *_Nonnull progress))progress
            success:(nullable void (^)())success
            failure:(nullable void (^)())failure;

/**
 *  Obtains the  video url and poster image url for the video with the videoPressID
 *
 *  @param videoPressID ID of video in VideoPress
 *  @param blog         blog to use to access video references
 *  @param success      return block if videopress info is found
 *  @param failure      return block if not information found.
 */
- (void)getMediaURLFromVideoPressID:(nonnull NSString *)videoPressID
                             inBlog:(nonnull Blog *)blog
                            success:(nullable void (^)(NSString * _Nonnull videoURL, NSString * _Nullable posterURL))success
                            failure:(nullable void (^)(NSError * _Nonnull error))failure;
/**
 * Sync all Media objects from the server to local database
 
 * @param blog
 * @param success a block that will be invoked when the sync succeeds
 * @param failure a block that will be invoked when the sync fails
 */
- (void)syncMediaLibraryForBlog:(nonnull Blog *)blog
                        success:(nullable void (^)())success
                        failure:(nullable void (^)(NSError * _Nonnull error))failure;

/**
 Gets a local thumbnail image file URL for the Media item, or generates one, if available.

 @discussion If the media asset is a video a frame of the video is returned.

 @param mediaInRandomContext the Media object from where to get the thumbnail.
 @param preferredSize the preferred size for the image in points. If set to CGSizeZero the resulting image will not
        exceed the maximum dimension of the UIScreen size.
 @param completion block that will be invoked when the thumbnail is ready, if available, or an error if something went wrong.
 */
- (void)thumbnailFileURLForMedia:(nonnull Media *)mediaInRandomContext
                   preferredSize:(CGSize)preferredSize
                      completion:(nonnull void (^)(NSURL * _Nullable url, NSError * _Nullable error))completion;
/**
 Gets a thumbnail image for the Media item, or generates one, if available.
 
 @discussion If the media asset is a video a frame of the video is returned.

 @param mediaInRandomContext the Media object from where to get the thumbnail.
 @param preferredSize the preferred size for the image in points. If set to CGSizeZero the resulting image will not
        exceed the maximum dimension of the UIScreen size.
 @param completion block that will be invoked when the thumbnail is ready, if available, or an error if something went wrong.
 */
- (void)thumbnailImageForMedia:(nonnull Media *)mediaInRandomContext
                 preferredSize:(CGSize)preferredSize
                    completion:(nonnull void (^)(UIImage * _Nullable image, NSError * _Nullable error))completion;
/**
 *  Get the number of items in a blog media library that are of a certain type.
 *
 *  @param blog from what blog to count the media items.
 *  @param mediaTypes set of media type values to be considered in the counting.
 */
- (NSInteger)getMediaLibraryCountForBlog:(nonnull Blog *)blog
                           forMediaTypes:(nonnull NSSet *)mediaTypes;

- (void)getMediaLibraryServerCountForBlog:(nonnull Blog *)blog
                            forMediaTypes:(nonnull NSSet *)mediaTypes
                                  success:(nullable void (^)(NSInteger count))success
                                  failure:(nullable void (^)(NSError * _Nonnull error))failure;

@end
