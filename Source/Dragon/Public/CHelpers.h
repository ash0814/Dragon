#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

//x 가 true면 return
#define CheckTreu(x) { if(x == true) return; }
//x가 true면 y를 return
#define CheckTrueResult(x, y) { if(x == true) return y; }

//x가 false면 retrun
#define CheckFalse(x) { if(x == false) return; }
//x가 false면 y를 return
#define CheckFalseResult(x, y) { if(x == false) return y;}

//x가 nullptr이면 return
#define CheckNUll(x) { if(x == nullptr) return; }
//x가 nullptr이면 y를 return
#define CheckNullResult(x, y) { if(x == nullptr) return y;}

//UE_LOG에서 해당 LOG를 호출한 함수와 .h/.cpp 에서 몇번째 줄에서 호출되는지를 확인하는 Log
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
//LOG_CALLINFO를 바탕으로 로그수준만 써주만 바로 출력
#define LOG_S(Verbosity) UE_LOG(LogTemp, Verbosity, TEXT("%s"), *LOG_CALLINFO)
//LOG_CALLINFO + TEXT
#define LOG(Verbosity, Format, ...) UE_LOG(LogTemp, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, FString::Printf(Format, ##__VA_ARGS__))
//Display에 출력시키는 로그
#define LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__))

 

class DRAGON_API CHelpers
{
public:

};
