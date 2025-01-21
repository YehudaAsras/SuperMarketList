//
//  ClubMenber.h
//  SuperMarketSol
//
//  Created by Yehuda Asras on 14/01/2025.
//

#pragma once
#include "Customer.h"

typedef struct {
    Customer base;
    int totalMonths;
} ClubMember;

int         initClubMember(ClubMember* pMember);
void        printClubMember(const ClubMember* pMember);
float       calculateDiscount(const ClubMember* pMember);
void        freeClubMember(ClubMember* pMember);
