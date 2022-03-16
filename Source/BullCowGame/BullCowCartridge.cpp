// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include "Math/UnrealMathUtility.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    valid=GetValidWords(List);
    InitGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else
        ProcessGuess(Input);
}
void UBullCowCartridge::InitGame()
{
    PrintLine(TEXT("Welcome to the Bull Cows game!"));
 
    HiddenWord =valid[FMath::RandRange(0,valid.Num())-1];
    Lives = HiddenWord.Len(); //SetLives
    bGameOver = false;
    PrintLine(TEXT("The HiddenWord is %i charcters long \nyou also have %i lives!"), HiddenWord.Len(),Lives);
    PrintLine(TEXT("Type in you're guess and press Enter!"));

}
void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Game has ended! press Enter to play again"));
}
void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Correct! fucking nailed it"));
        EndGame();
        return; 
    }
    if(Guess=="h")
    {
        size_t i=FMath::RandRange(0,HiddenWord.Len());
        PrintLine(TEXT("The %i char is '%c"),i+1,HiddenWord[i]);
        return;
    }
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repetting letters! guess again"));
        return;
    }
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The HiddenWord is %i characters long! Try again!"), HiddenWord.Len());
        return;
    }
    --Lives;
    if (Lives == 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have died :( ,The word was:[%s]"), *HiddenWord);
        EndGame();
        return;
    }
    int32 Bulls,Cows;
    GetBullCows(Guess,Bulls,Cows);
    PrintLine(TEXT("Wrong!Lost 1 live!\nYou have [%i] lives remaining"),Lives);
    PrintLine(TEXT("You Have %i Bulls and %i Cows\nFor a clue Enter 'h'"),Bulls,Cows);

}
bool UBullCowCartridge::IsIsogram(const FString &Word)const
{
    for (size_t i = 0; i < Word.Len(); i++)
    {
        for (size_t j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
                return false;
        }
    }
    return true;
}
TArray<FString>UBullCowCartridge::GetValidWords(const TArray<FString> &list)const
{
    TArray<FString>tmp;
    for(auto var: List)
    {
        if(var.Len()>=3&&var.Len()<=5 && IsIsogram(var))
            tmp.Emplace(var);
    }
    return tmp;
}
void UBullCowCartridge::GetBullCows(const FString &Guess,int32&BullCount,int32&CowCount)const
{
    BullCount=0;
    CowCount=0;
    for(size_t i=0;i<Guess.Len();i++)
    {
        if(Guess[i]==HiddenWord[i])
        {
            BullCount++;
            continue;
        }
        for(size_t j=0;j<Guess.Len();j++)
        {
            if(Guess[i]==HiddenWord[j])
            {
                CowCount++;
                break;
            }

        }
    }
}
