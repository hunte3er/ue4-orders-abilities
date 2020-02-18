# fpwong branch of ue4-orders-abilities

Basic implementation of the orders-abilities system. Code is in a mess, use for reference mainly.

Combat system generally works but I need to improve how characters store and use abilities

In other works you can only basic attack

Most assets are not included, so expect to replace some of the AnimMontages and AnimBP if you want to get the project running. 

You can test the Spider / Snake / Wasp units are free from [Quaternius](http://quaternius.com)

Significant changes:
* TargetType uses flags (aka can define multiple target types like ACTOR or LOCATION for an order)
* Blackboard helper was changed slightly to have better support in C++ as oppose to Blueprints
* Included some code from [Nick's RTS project](https://github.com/npruehs/ue4-rts)

---

# Building the project

1. Clone the repo: `git clone https://github.com/fpwong/ue4-orders-abilities.git`
2. Change dir: `cd ue4-orders-abilities`
3. Checkout the fpwong branch: `git checkout fpwong`
4. Clone the helper plugin into the Plugins folder: `git clone https://github.com/fpwong/FPGameplayAbilities.git Plugins/FPGameplayAbilities`
5. Open the .uproject

# Getting the project running

1. Select a skeleton for AnimBP_Mage
1. Open the BP_OrdersAbilitiesGameMode and set the InitialActors array to a single BP_Spider