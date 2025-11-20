#include <stdio.h>
double installment,extrapayment;
int totalyears, currentyear;

double calculateRepayment(double loan, double interestRate, int years){
    if(loan<0||years==0){
        return 0;
    }
    loan+=(loan*interestRate/100);
    loan-=(installment+extrapayment);
    if (loan < 0){  
        loan = 0;
    }    
    currentyear=totalyears-years+1;
    printf("Year %d: Remaining loan = %.2f\n", currentyear, loan);
    return (installment+extrapayment)+ calculateRepayment(loan, interestRate, years - 1);
}
int main(){
    double loan = 100000;
    double interestRate = 5; 
    int years = 3;
    extrapayment=5000;
    totalyears = years;  
    installment = loan / years;

    double totalPaid = calculateRepayment(loan, interestRate, years);

    printf("\nTotal repayment = %.2f\n", totalPaid);

    return 0;
}
