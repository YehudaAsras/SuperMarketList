//
//  ClubMember.c
//  SuperMarketSol
//
//  Created by Yehuda Asras on 14/01/2025.
//
#include <stdio.h>
#include "../Hfolder/ClubMenber.h"
#include "../Hfolder/General.h"

int initClubMember(ClubMember* pMember) {
    if (!pMember) return 0;

    if (!initRegularCustomer(&pMember->base)) return 0;
    
    pMember->base.type = CLUB_MEMBER;
    
    pMember->totalMonths = getPositiveInt("Enter total membership months:\t");

    return 1;
}

float calculateDiscount(const ClubMember* pMember) {
    if (!pMember) return 0.0;

    int totalMonths = pMember->totalMonths;
    float discount = 0.0;

    // Calculate discount based on membership duration
    if (totalMonths <= 24) {
        // Up to 2 years: 0.1% per month, max 2.3%
        discount = 0.1 * totalMonths;
        if (discount > 2.3) discount = 2.3;
    } else if (totalMonths <= 60) {
        // Between 2 and 5 years: base 2.3% + 0.5% per year
        discount = 2.3 + 0.5 * ((totalMonths - 24) / 12);
        if (discount > 4.5) discount = 4.5;
    } else {
        // Over 5 years: fixed 7.5%
        discount = 7.5;
    }

    return discount;
}

void printClubMember(const ClubMember* pMember) {
    if (!pMember) return;

    printCustomer(&pMember->base);

    printf("Customer type: Club member\n");
    printf("Membership months: %d\n", pMember->totalMonths);
}

void freeClubMember(ClubMember* pMember) {
    if (!pMember) return;

    freeCustomer(&pMember->base);
}
