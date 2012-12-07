//
//  NativeAddressBook.m
//  Proj1_1
//
//  Created by Nathan brummel on 9/20/12.
//  Copyright (c) 2012, Berkeley. All rights reserved.
//

#import "NativeAddressBook.h"

#import <AddressBook/AddressBook.h>
#import <JavaNativeFoundation/JavaNativeFoundation.h> // helper framework for Cocoa and JNI development

#import "com_example_app_addressbook_NativeAddressBook.h" // generated from NativeAddressBook.java


/* 
 * Entry point from Java though JNI to call into the Mac OS X Address Book framework,
 * and create Java objects from the Objective-C Foundation objects returned from the
 * Address Book.
 * 
 * Uses JavaNativeFoundation, a sub-framework of the JavaVM.framework to setup autorelease
 * pools, catch NSExceptions, and re-throw them as Java exceptions. JNF provides a layer
 * on top of conventional C-based JNI to ease interoperability with Cocoa.
 *
 * For more information on conventional JNI on Mac OS X, see:
 * <http://developer.apple.com/technotes/tn2005/tn2147.html>
 */


// coerces an ABPerson into a map
@interface ABPersonCoercer : NSObject<JNFTypeCoercion> { }
+ (ABPersonCoercer *) personCoercer;
@end

// coerces an ABMultiValue into a list of single key/value maps
@interface ABMultiValueCoercer : NSObject<JNFTypeCoercion> { }
+ (ABMultiValueCoercer *) multiValueCoercer;
@end


/*
 * Class:     com_example_app_addressbook_NativeAddressBook
 * Method:    getMyUID
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_app_addressbook_NativeAddressBook_getMyUID
(JNIEnv *env, jclass clazz)
{
	jstring myUID = NULL; // need to declare outside of the JNF_COCOA_ENTER/EXIT @try/catch scope
	
JNF_COCOA_ENTER(env);
	
	ABPerson *me = [[ABAddressBook sharedAddressBook] me];
	myUID = JNFNSToJavaString(env, [me uniqueId]); // convert the NSString to a Java string
	
JNF_COCOA_EXIT(env);
	
	return myUID;
}


/*
 * Class:     com_example_app_addressbook_NativeAddressBook
 * Method:    getNativeAddressBookContacts
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_example_app_addressbook_NativeAddressBook_getNativeAddressBookContacts
(JNIEnv *env, jclass clazz)
{
    jobject persons = NULL; // need to declare outside of the JNF_COCOA_ENTER/EXIT @try/catch scope
	
JNF_COCOA_ENTER(env);
	
	// get all people into an array
	ABAddressBook *addressBook = [ABAddressBook sharedAddressBook];
	NSMutableArray *people = [NSMutableArray arrayWithArray:[addressBook people]];
	
	// add yourself, because -people does not vend the Me card.
	ABPerson *me = [addressBook me];
	if (me != nil) [people addObject:me];
	
	// create and load a coercer with all of the different coercions to convert each type of object
	JNFTypeCoercer *coercer = [[[JNFTypeCoercer alloc] init] autorelease];
	[JNFDefaultCoercions addStringCoercionTo:coercer];
	[JNFDefaultCoercions addNumberCoercionTo:coercer];
	[JNFDefaultCoercions addListCoercionTo:coercer];
	[JNFDefaultCoercions addMapCoercionTo:coercer];
	[JNFDefaultCoercions addSetCoercionTo:coercer];
	[coercer addCoercion:[ABPersonCoercer personCoercer] forNSClass:[ABPerson class] javaClass:nil];
	[coercer addCoercion:[ABMultiValueCoercer multiValueCoercer] forNSClass:[ABMultiValue class] javaClass:nil];
	
	// recursively decend into the object graph of "people", and 
	// convert every NSObject into a corresponding Java object
	persons = [coercer coerceNSObject:people withEnv:env];
	
JNF_COCOA_EXIT(env);
	
	return persons;
}


// puts all the properties of an ABPerson into an NSDictionary, and uses the same 
// root coercer to transform that NSDictionary into a java.util.Map
@implementation ABPersonCoercer

+ (ABPersonCoercer *) personCoercer {
	return [[[ABPersonCoercer alloc] init] autorelease];
}

- (jobject) coerceNSObject:(id)obj withEnv:(JNIEnv *)env usingCoercer:(JNFTypeCoercion *)coercer {
	ABPerson *person = obj;
	NSMutableDictionary *dict = [NSMutableDictionary dictionary];
	
	NSArray *props = [ABPerson properties];
	for (NSString *propName in props) {
		if (propName == nil) continue;
		
		id prop = [person valueForProperty:propName]; 
		if (prop == nil) continue;
		
		[dict setValue:prop forKey:propName];
	}
	
	return [coercer coerceNSObject:dict withEnv:env usingCoercer:coercer];
}

- (id) coerceJavaObject:(jobject)obj withEnv:(JNIEnv *)env usingCoercer:(JNFTypeCoercion *)coercer {
	return nil; // exercise left to the reader
}

@end


// creates an NSArray for each ABMultiValue, and puts each key/value pair into its own NSDictionary,
// then uses the same root coercer to transform them into java.util.Lists and java.util.Maps
@implementation ABMultiValueCoercer

+ (ABMultiValueCoercer *) multiValueCoercer {
	return [[[ABMultiValueCoercer alloc] init] autorelease];
}

- (jobject) coerceNSObject:(id)obj withEnv:(JNIEnv *)env usingCoercer:(JNFTypeCoercion *)coercer {
	ABMultiValue *multiValue = obj;
	
	NSUInteger count = [multiValue count];
	NSMutableArray *values = [NSMutableArray arrayWithCapacity:count];
	
	NSUInteger i;
	for (i = 0; i < count; i++) {
		NSString *label = [multiValue labelAtIndex:i];
		id object = [multiValue valueAtIndex:i];
		
		[values addObject:[NSDictionary dictionaryWithObjects:&object forKeys:&label count:1]];
	}
	
	return [coercer coerceNSObject:values withEnv:env usingCoercer:coercer];
}

- (id) coerceJavaObject:(jobject)obj withEnv:(JNIEnv *)env usingCoercer:(JNFTypeCoercion *)coercer {
	return nil; // exercise left to the reader
}

@end
