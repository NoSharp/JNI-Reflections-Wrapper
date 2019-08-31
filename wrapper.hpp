/*
	MIT LICENSE. Written by NoSharp or Harry Kerr, Please use if you need to.
*/


#pragma once

#include <iostream>
#include "jni.h"
#include <map>


using namespace std;

/*
	Made this because the shit one I use in interfacing is completely fucked.
*/
jclass getClassByName(JNIEnv* env, const char* qualifiedName)
{

	// Get the class from the JNIEnvironment.
	jclass clazz = env->FindClass(qualifiedName);


	return clazz;
}

/*

	Written by Harry Kerr (C) 
	Feel free to use wherever ( this clause has limitation to legal only applications ) but please credit me.

*/
class jReflectionsWrapper
{
	private:
		JNIEnv* env;
		jclass reflectionsClass;

	public:
		/*
			Takes one parameter, the JNI instance gotten via Javaw instance.
		*/
		jReflectionsWrapper(JNIEnv* env) {

		
			this->env = env;

			this->reflectionsClass = env->FindClass("java/lang/reflect/Method");

			if (this->reflectionsClass == NULL) {
		
				cout << "Can't find reflections class." << endl;
			}

			cout << "REFLECTIONS CLASS POINTER: " << this->reflectionsClass << endl;
		}

		/*
			Deletes the pointer(s).
		*/
		~jReflectionsWrapper() 
		{
			delete(this->env);
			delete(this->reflectionsClass);
			
		}
	
		/*
			get's all methods in a class.
		*/
		map<string, jobject> *getMethods(jobject obj)
		{	
			jclass cls = env->GetObjectClass(obj);

			// Get methods
			jmethodID mid = env->GetMethodID(cls, "getClass", "()Ljava/lang/Class;");
			jobject clsObj = env->CallObjectMethod(obj, mid);

			
			// Returns an array of methods
			jmethodID methodID = this->env->GetMethodID(cls, "getDeclaredMethods", "()[Ljava/lang/reflect/Method;");
			jobjectArray methodArray = (jobjectArray)this->env->CallObjectMethod(clsObj, methodID);

			

			int methodLength = this->env->GetArrayLength(methodArray);
			string* methods = new string[methodLength];

			
			map<string, jobject>* stringLookupTable = new map<string, jobject>();

			for (int i = 0; i < methodLength; i++) {
			
				jobject jOb = (jobject)this->env->GetObjectArrayElement(methodArray, i);
				
				jmethodID fId = this->env->GetMethodID(this->reflectionsClass, "getName", "()Ljava/lang/String;" );

				jstring name = (jstring)this->env->CallObjectMethod(jOb, fId);

				string normalized = env->GetStringUTFChars(name, 0);

				stringLookupTable->insert(pair<string, jobject>(normalized, jOb));

			}
			
			return stringLookupTable;
		}

};