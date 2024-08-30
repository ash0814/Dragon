#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

//x �� true�� return
#define CheckTreu(x) { if(x == true) return; }
//x�� true�� y�� return
#define CheckTrueResult(x, y) { if(x == true) return y; }

//x�� false�� retrun
#define CheckFalse(x) { if(x == false) return; }
//x�� false�� y�� return
#define CheckFalseResult(x, y) { if(x == false) return y;}

//x�� nullptr�̸� return
#define CheckNUll(x) { if(x == nullptr) return; }
//x�� nullptr�̸� y�� return
#define CheckNullResult(x, y) { if(x == nullptr) return y;}

//UE_LOG���� �ش� LOG�� ȣ���� �Լ��� .h/.cpp ���� ���° �ٿ��� ȣ��Ǵ����� Ȯ���ϴ� Log
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
//LOG_CALLINFO�� �������� �α׼��ظ� ���ָ� �ٷ� ���
#define LOG_S(Verbosity) UE_LOG(LogTemp, Verbosity, TEXT("%s"), *LOG_CALLINFO)
//LOG_CALLINFO + TEXT
#define LOG(Verbosity, Format, ...) UE_LOG(LogTemp, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, FString::Printf(Format, ##__VA_ARGS__))
//Display�� ��½�Ű�� �α�
#define LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(Format, ##__VA_ARGS__))

 

class DRAGON_API CHelpers
{
public:

};
