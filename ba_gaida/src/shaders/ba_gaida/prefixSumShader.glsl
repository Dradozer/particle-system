#version 450
/*
 * 1.2 ComputeShader
 * Calc PreFixSum of Grid
 */
layout(local_size_x = 10, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    vec4 position;
    vec4 velocity;
    uint gridID;
    uint memoryPosition;
    uint pad2, pad3;
};

struct Grid{
    uint gridID;
    uint particlesInGrid;
    uint previousSortOutPut;
    uint currentSortOutPut;
};

layout(std430, binding = 0) readonly buffer buffer_particle1
{
    Particle particle1[];
};

layout(std430, binding = 1) writeonly buffer buffer_particle2
{
    Particle particle2[];
};

layout(std430, binding = 2) coherent buffer buffer_grid
{
    Grid grid[];
};

uniform float deltaTime;
uniform uint particleCount;
uniform ivec4 gridSize;

//int cubeid;
const uint maxCubes = (gridSize.x * gridSize.y * gridSize.z);
//uint ITTERATIONS = uint(ceil(log2(maxCubes)));
//shared int temp[1000];

void main(void) {

    uint id = gl_GlobalInvocationID.x;
    const int threadID = int (id);
    //    int pout = 0, pin = 1;
    //    int diffID = 1;
    //    if (threadID > 0){
    //        temp[threadID] = grid[threadID -1].particlesInGrid;
    //    } else
    //    {
    //        temp[threadID] = 0;
    //    }

    //    memoryBarrierShared();
    if (threadID >= maxCubes)
    {
        return;
    } else
    {
        //        for(int i = 1; i < 1000; i <<= 1){
        //            pout = 1 - pout;
        //            pin = 1 - pin;
        //
        //            if(threadID >= i)
        //                temp[pout *1000 + threadID] += temp[pin * 1000 + threadID - i];
        //            else
        //                temp[pout*1000 + threadID] = temp[pin * 1000 + threadID];
        //
        //            memoryBarrierShared();
        //        }
        //
        //    grid[threadID].currentSortOutPut = temp[pout * 1000 + threadID];

        //        memoryBarrierBuffer();
        //            grid[threadID].previousSortOutPut = grid[threadID].particlesInGrid;
        //            for (int i = 0; i <= ITTERATIONS ;i++)
        //            {
        //                memoryBarrierBuffer();
        //                if(threadID >= diffID){
        //                    atomicAdd(grid[threadID].currentSortOutPut,grid[threadID - diffID].previousSortOutPut);
        //                }
        //                memoryBarrierBuffer();
        //                grid[threadID].previousSortOutPut = grid[threadID].currentSortOutPut;
        //                diffID = diffID * 2;
        //                memoryBarrierBuffer();
        //            }
        //        memoryBarrierBuffer();

        for (int i = 0; i < id; i++){
            grid[id].currentSortOutPut += grid[i].particlesInGrid;
        }
    grid[id].previousSortOutPut = grid[id].currentSortOutPut + grid[id].particlesInGrid;
    }
}