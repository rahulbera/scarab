# Scarab Power Model

## Enabling Scarab Power Model
### 1. Download McPAT
The scarab power model relies on McPAT. Scarab has been tested and works with McPAT v1.0, which
can be found [here](https://github.com/HewlettPackard/mcpat).

### 2. Download CACTI
The scarab power model relies on CACTI. Scarab has been tested and works with CACTI 6.5, which
can be found [here](https://github.com/HewlettPackard/cacti).
Note: checkout the v6.5.0 of CACTI. v7 onwards has uninitialized variable bugs,
which produce undefined behaviour (and worst case, seg fault).

### 3. Apply patches
Apply the patches provided to the respective tool.

* McPat:

>$ cd <mcpat>
>$ git apply --stat <scarab\_root>/bin/power/mcpat.patch
>$ git apply --check <scarab\_root>/bin/power/mcpat.patch
>$ git am < <scarab\_root>/bin/power/mcpat.patch
>$ make -j

* Cacti:

>$ cd <cacti>
>$ git apply --stat <scarab\_root>/bin/power/cacti.patch
>$ git apply --check <scarab\_root>/bin/power/cacti.patch
>$ git am < <scarab\_root>/bin/power/cacti.patch
>$ make -j

### 4. Set Environment Variables
Scarab uses ```MCPAT_BIN``` and ```CACTI_BIN``` to find McPat and CACTI binaries. You can use the following commands:
>$ export MCPAT\_BIN=<mcpat\_root\_dir>/mcpat
>$ export CACTI\_BIN=<cacti\_root\_dir>/cacti

By default, Scarab looks for McPat and CACTI binaries in the paths specified by ```__mcpat_bin__``` and ```__cacti_bin__```, which are defined in ```<scarab_root>/bin/scarab_globals/scarab_paths.py```. Changing these variables is another way you can set the paths to the power tools.

### 5. Enabling Power Simulation
```power_intf_on``` enables the power simulation and it can be enabled in the PARAM file or in the command-line arguments when launching Scarab.


### 6. Enabling Dynamic Voltage-and-Frequency Scaling (DVFS):
Scarab supports DVFS with the following changes to McPAT and CACTI. These changes are supplied in two patch files, mcpat.patch and cacti.patch. To apply the patches, first download McPAT and CACTI (follow the directions above), then apply the patches using as below.


## Power Model Flow
Important power model files can be found under two directories:
1. ```<scarab_root>/bin/power/```
2. ```<scarab_root>/src/power/```
