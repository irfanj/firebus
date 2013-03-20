/*
 * Firebase iOS Client Library
 *
 * Copyright © 2013 Firebase - All Rights Reserved
 * https://www.firebase.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binaryform must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY FIREBASE AS IS AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL FIREBASE BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#import <Foundation/Foundation.h>
#import "FQuery.h"
#import "FDataSnapshot.h"
#import "FMutableData.h"
#import "FTransactionResult.h"

/**
 * A Firebase reference represents a particular location in your Firebase
 * and can be used for reading or writing data to that Firebase location.
 *
 * This class is the starting point for all Firebase operations. After you've
 * initialized it with initWithUrl: you can use it
 * to read data (ie. observeEventType:withBlock:), write data (ie. setValue:), and to create new
 * Firebase references (ie. child:).
 */
@interface Firebase : FQuery

+ (void) setDispatchQueue:(dispatch_queue_t)queue;

+ (NSString *) sdkVersion;


/**
 * Initialize this Firebase reference with an absolute URL. 
 *
 * @param url The Firebase URL (ie: https://SampleChat.firebaseIO-demo.com)
 */
- (id)initWithUrl:(NSString *)url;


/**
 * Get a Firebase reference for the location at the specified relative path.
 * The relative path can either be a simple child name (e.g. 'fred') or a 
 * deeper slash-separated path (e.g. 'fred/name/first').
 *
 * @param pathString A relative path from this location to the desired child location.
 * @return A Firebase reference for the specified relative path.
 */
- (Firebase *) childByAppendingPath:(NSString *)pathString;


/**
 * childByAutoId generates a new child location using a unique name and returns a
 * Firebase reference to it. This is useful when the children of a Firebase
 * location represent a list of items.
 *
 * The unique name generated by childByAutoId: is prefixed with a client-generated
 * timestamp so that the resulting list will be chronologically-sorted.
 *
 * @return A Firebase reference for the generated location.
 */
- (Firebase *) childByAutoId;


/**
 * Gets the absolute URL of this Firebase location. 
 *
 * @return The absolute URL of the referenced Firebase location.
 */
- (NSString *) description;

// Writing Data

/**
 * Write data to this Firebase location. This will overwrite any data 
 * at this location and all child locations. 
 *
 * Data types that can be set are:
 * * NSString -- @"Hello World"
 * * NSNumber (also includes boolean) -- @YES, @43, @4.333
 * * NSDictionary -- @{@"key": @"value"}
 * * NSArray
 *
 * The effect of the write will be visible immediately and the corresponding
 * events will be triggered. Synchronization of the data to the Firebase 
 * servers will also be started.
 *
 * Passing null for the new value is equivalent to calling remove:;
 * all data at this location or any child location will be deleted.
 *
 * Note that setValue: will remove any priority stored at this location, so if priority
 * is meant to be preserved, you should use setValue:andPriority: instead.
 *
 * @param value The value to be written.
 */
- (void) setValue:(id)value;


/**
 * The same as setValue: with a block that gets triggered after the write operation has
 * been committed to the Firebase servers.
 *
 * @param value The value to be written.
 * @param block The block to be called after the write has been committed to the Firebase servers.
 */
- (void) setValue:(id)value withCompletionBlock:(void (^)(NSError* error))block;


/**
 * The same as setValue: with an additional priority to be attached to the data being written.
 * Priorities are used to order items.
 *
 * @param value The value to be written.
 * @param priority The priority to be attached to that data.
 */
- (void) setValue:(id)value andPriority:(id)priority;


/**
 * The same as setValue:andPriority: with a block that gets triggered after the write operation has
 * been committed to the Firebase servers.
 *
 * @param value The value to be written.
 * @param priority The priority to be attached to that data.
 * @param block The block to be called after the write has been committed to the Firebase servers.
 */
- (void) setValue:(id)value andPriority:(id)priority withCompletionBlock:(void (^)(NSError* error))block;


/**
 * Remove the data at this Firebase location. Any data at child locations will also be deleted.
 * 
 * The effect of the delete will be visible immediately and the corresponding events
 * will be triggered. Synchronization of the delete to the Firebase servers will 
 * also be started.
 *
 * remove: is equivalent to calling setValue:nil
 */
- (void) removeValue;


/**
 * The same as remove: with a block that gets triggered after the remove operation has
 * been committed to the Firebase servers.
 *
 * @param block The block to be called after the remove has been committed to the Firebase servers.
 */
- (void) removeValueWithCompletionBlock:(void (^)(NSError* error))block;


/**
 * Set a priority for the data at this Firebase location.
 * Priorities can be used to provide a custom ordering for the children at a location
 * (if no priorities are specified, the children are ordered by name).
 *
 * You cannot set a priority on an empty location. For this reason
 * setValue:andPriority: should be used when setting initial data with a specific priority
 * and setPriority: should be used when updating the priority of existing data.
 *
 * Children are sorted based on this priority using the following rules:
 *
 * Children with no priority (a null priority) come first. They are ordered lexicographically by name.
 * Children with a priority that is parsable as a number come next. They are 
 * sorted numerically by priority first (small to large) and lexicographically by name second (A to z).
 * Children with non-numeric priorities come last. They are sorted lexicographically 
 * by priority first and lexicographically by name second.
 * Setting the priority to null removes any existing priority. 
 * Note that priorities are parsed and ordered as IEEE 754 double-precision floating-point numbers.
 *
 * @param priority The priority to set at the specified location.
 */
- (void) setPriority:(id)priority;


/**
 * The same as setPriority: with a block block that is called once the priority has
 * been committed to the Firebase servers.
 *
 * @param priority The priority to set at the specified location.
 * @param block The block that is triggered after the priority has been written on the servers.
 */
- (void) setPriority:(id)priority withCompletionBlock:(void (^)(NSError* error))block;


/**
 * Update changes the values of the keys specified in the dictionary without overwriting other
 * keys at this location.
 *
 * @param value A dictionary of the keys to change and their new values
 */
- (void) updateChildValues:(NSDictionary *)values;


/**
 * The same as update: with a block block that is called once the update has been committed to the 
 * Firebase servers
 *
 * @param value A dictionary of the keys to change and their new values
 * @param block The block that is triggered after the update has been written on the Firebase servers
 */
- (void) updateChildValues:(NSDictionary *)values withCompletionBlock:(void (^)(NSError* error))block;


// Reading data


/**
 * observeEventType:withBlock: is used to listen for data changes at a particular location.
 * This is the primary way to read data from Firebase. Your block will be triggered
 * for the initial data and again whenever the data changes.
 *
 * Use removeObserverWithHandle: to stop receiving updates.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called with initial data and updates.
 * @return A handle used to unregister this block later using removeObserverWithHandle:
 */
- (FirebaseHandle) observeEventType:(FEventType)eventType withBlock:(void (^)(FDataSnapshot* snapshot))block;


- (FirebaseHandle) observeEventType:(FEventType)eventType andPreviousSiblingNameWithBlock:(void (^)(FDataSnapshot* snapshot, NSString* prevName))block;


- (FirebaseHandle) observeEventType:(FEventType)eventType withBlock:(void (^)(FDataSnapshot* snapshot))block withCancelBlock:(void (^)(void))cancelBlock;


- (FirebaseHandle) observeEventType:(FEventType)eventType andPreviousSiblingNameWithBlock:(void (^)(FDataSnapshot* snapshot, NSString* prevName))block withCancelBlock:(void (^)(void))cancelBlock;


/**
 * Detach a block previously attached with observeEventType:withBlock:. 
 *
 * @param handle The handle returned by the call to on: which we are trying to cancel.
 */
- (void) removeObserverWithHandle:(FirebaseHandle)handle;


/**
 * Detach all blocks previously attached to this Firebase location with observeEventType:withBlock:
 */
- (void) removeAllObservers;


/**
 * This is equivalent to observeEventType:withBlock:, except the block is immediately canceled after the initial data is returned.
 *
 * @param eventType The type of event to listen for.
 * @param block The block that should be called with initial data and updates.
 */
- (void) observeSingleEventOfType:(FEventType)eventType withBlock:(void (^)(FDataSnapshot* snapshot))block;


- (void) observeSingleEventOfType:(FEventType)eventType andPreviousSiblingNameWithBlock:(void (^)(FDataSnapshot* snapshot, NSString* prevName))block;


- (void) observeSingleEventOfType:(FEventType)eventType withBlock:(void (^)(FDataSnapshot* snapshot))block withCancelBlock:(void (^)(void))cancelBlock;


- (void) observeSingleEventOfType:(FEventType)eventType andPreviousSiblingNameWithBlock:(void (^)(FDataSnapshot* snapshot, NSString* prevName))block withCancelBlock:(void (^)(void))cancelBlock;


- (FQuery *) queryStartingAtPriority:(id)startPriority;


- (FQuery *) queryStartingAtPriority:(id)startPriority andChildName:(NSString *)childName;


- (FQuery *) queryEndingAtPriority:(id)endPriority;


- (FQuery *) queryEndingAtPriority:(id)endPriority andChildName:(NSString *)childName;


- (FQuery *) queryLimitedToNumberOfChildren:(NSUInteger)limit;


// Operations to be run on disconnect

/**
 * Ensure the data at this location is set to the specified value when
 * the client is disconnected (due to closing the browser, navigating
 * to a new page, or network issues).
 *
 * onDisconnectSetValue: is especially useful for implementing "presence" systems,
 * where a value should be changed or cleared when a user disconnects
 * so that he appears "offline" to other users.
 *
 * @param value The value to be set after the connection is lost.
 */
- (void) onDisconnectSetValue:(id)value;


- (void) onDisconnectSetValue:(id)value withCompletionBlock:(void (^)(NSError* error))block;


- (void) onDisconnectSetValue:(id)value andPriority:(id)priority;


- (void) onDisconnectSetValue:(id)value andPriority:(id)priority withCompletionBlock:(void (^)(NSError* error))block;


/**
 * Ensure the data at this location is removed when
 * the client is disconnected (due to closing the app, navigating
 * to a new page, or network issues).
 *
 * onDisconnectRemoveValue: is especially useful for implementing "presence" systems.
 */
- (void) onDisconnectRemoveValue;


- (void) onDisconnectRemoveValueWithCompletionBlock:(void (^)(NSError* error))block;


- (void) onDisconnectUpdateChildValues:(NSDictionary *)values;


- (void) onDisconnectUpdateChildValues:(NSDictionary *)values withCompletionBlock:(void (^)(NSError* error))block;


- (void) cancelDisconnectOperations;


- (void) cancelDisconnectOperationsWithCompletionBlock:(void (^)(NSError* error))block;


// Authentication operations


- (void) authWithCredential:(NSString *)credential withCompletionBlock:(void (^) (NSError* error, id data))block withCancelBlock:(void (^)(NSError* error))cancelBlock;


- (void) unauth;


// Transaction methods


- (void) runTransactionBlock:(FTransactionResult* (^) (FMutableData* currentData))block;


- (void) runTransactionBlock:(FTransactionResult* (^) (FMutableData* currentData))block andCompletionBlock:(void (^) (NSError* error, BOOL committed, FDataSnapshot* snapshot))completionBlock;


- (void) runTransactionBlock:(FTransactionResult* (^) (FMutableData* currentData))block andCompletionBlock:(void (^) (NSError* error, BOOL committed, FDataSnapshot* snapshot))completionBlock withLocalEvents:(BOOL)localEvents;


/**
 * Get a Firebase reference for the parent location.
 * If this instance refers to the root of your Firebase, it has no parent,
 * and therefore parent( ) will return null.
 *
 * @return A Firebase reference for the parent location.
 */
@property (strong, readonly, nonatomic) Firebase* parent;


@property (strong, readonly, nonatomic) Firebase* root;


/**
 * Gets last token in a Firebase location (e.g. 'fred' in https://SampleChat.firebaseIO-demo.com/users/fred)
 *
 * @return The name of the location this reference points to.
 */
@property (strong, readonly, nonatomic) NSString* name;

@end