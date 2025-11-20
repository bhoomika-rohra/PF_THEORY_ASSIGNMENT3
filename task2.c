#include <stdio.h>
void calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets){
    if (fuel <= 0) {
        printf("Planet %d: Fuel Remaining = 0\n", planet);
        printf("Mission failed! Spacecraft ran out of fuel.\n");
        return;
    }
    if (planet > totalPlanets) {
        printf("Mission complete! Fuel remaining = %d\n", fuel);
        return;
    }
    fuel=fuel-consumption;
    fuel += recharge;
    if (planet % 4 == 0) {
        fuel += solarBonus;
    }
    if (fuel < 0){
        fuel = 0;
    }
    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);
    calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);

}
int main() {
    int fuel = 1000;
    int consumption = 120;
    int recharge = 30;
    int solarBonus = 100;
    int totalPlanets = 10;

    calculateFuel(fuel, consumption, recharge, solarBonus, 1, totalPlanets);

    return 0;
}
