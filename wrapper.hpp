#pragma once

#include <iostream>
#include "jni.h"
#include <map>

using namespace std;

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

			//cout << "REFLECTIONS CLASS POINTER: " << this->reflectionsClass << endl;
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
		map<string, jobject> *getMethods(jobject clazz)
		{	

			jclass cls = env->GetObjectClass(clazz);

			// Returns an array of methods
			jmethodID methodID = this->env->GetMethodID(cls, "getDeclaredMethods", "()[Ljava/lang/reflect/Method;");
			jobjectArray methodArray = (jobjectArray)this->env->CallObjectMethod(clazz, methodID);

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
		/*
			get's all Fields in a class.
		*/
		map<string, jobject>* getFields(jobject clazz)
		{

			jclass cls = env->GetObjectClass(clazz); // .class Equiv

			// Returns an array of methods

			jmethodID methodID = this->env->GetMethodID(cls, "getDeclaredFields", "()[Ljava/lang/reflect/Field;");
			if (methodID == NULL) {
				cout << "METHOIDID IS NIL." << endl;
			
			}
			//cout << methodID << endl;

			jobjectArray methodArray = (jobjectArray)this->env->CallObjectMethod(clazz, methodID);
			//cout << "METHOD ARRAY: " << methodArray << endl;

			int methodLength = this->env->GetArrayLength(methodArray);
			string* methods = new string[methodLength];

			map<string, jobject>* stringLookupTable = new map<string, jobject>();

			for (int i = 0; i < methodLength; i++) {

				jobject jOb = (jobject)this->env->GetObjectArrayElement(methodArray, i);
				
				//cout << "Java OBJECT: " << jOb << endl;;

				jmethodID fId = this->env->GetMethodID(this->reflectionsClass, "toString", "()Ljava/lang/String;");

				//cout << "FIELD ID: " << fId << endl;

				jstring name = (jstring)this->env->CallObjectMethod(jOb, fId);
				

				string normalized = env->GetStringUTFChars(name, 0);

				stringLookupTable->insert(pair<string, jobject>(normalized, jOb));

			}

			return stringLookupTable;
		}
};
