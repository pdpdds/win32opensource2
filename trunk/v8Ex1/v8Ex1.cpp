// v8Ex1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "include/v8.h"
#include "include/libplatform/libplatform.h"

#pragma comment(lib, "v8_base.lib")
#pragma comment(lib, "v8_libbase.lib")
#pragma comment(lib, "v8_libplatform.lib")
#pragma comment(lib, "v8_nosnapshot.lib")
#pragma comment(lib, "v8_snapshot.lib")
#pragma comment(lib, "icuuc.lib")
#pragma comment(lib, "icui18n.lib")

using namespace v8;

v8::Handle<v8::String> ReadFile(v8::Isolate* isolate, const char* name);

int main(int argc, char* argv[]) {
	// Initialize V8.
	V8::InitializeICU();
	Platform* platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();

	// Create a new Isolate and make it the current one.
	Isolate* isolate = Isolate::New();
	{
		Isolate::Scope isolate_scope(isolate);

		// Create a stack-allocated handle scope.
		HandleScope handle_scope(isolate);

		// Create a new context.
		Local<Context> context = Context::New(isolate);

		// Enter the context for compiling and running the hello world script.
		Context::Scope context_scope(context);

		// Create a string containing the JavaScript source code.
		Local<String> source = ReadFile(isolate, "sample.js");
		// Compile the source code.
		Local<Script> script = Script::Compile(source);

		// Run the script to get the result.
		Local<Value> result = script->Run();

		// Convert the result to an UTF8 string and print it.
		String::Utf8Value utf8(result);
		printf("%s\n", *utf8);
	}

	// Dispose the isolate and tear down V8.
	isolate->Dispose();
	V8::Dispose();
	V8::ShutdownPlatform();
	delete platform;
	return 0;
}

// Reads a file into a v8 string.
v8::Handle<v8::String> ReadFile(v8::Isolate* isolate, const char* name) {
	FILE* file;
	errno_t err = fopen_s(&file, name, "rb");
	if (err != 0) return v8::Handle<v8::String>();

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0; i < size;) {
		int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
		i += read;
	}
	fclose(file);
	v8::Handle<v8::String> result =
		v8::String::NewFromUtf8(isolate, chars, v8::String::kNormalString, size);
	delete[] chars;
	return result;
}