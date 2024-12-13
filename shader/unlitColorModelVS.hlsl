
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
    Out = (PS_IN) 0;
	
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Out.Position = mul(In.Position, wvp);
    if (Material.Diffuse.r == 0.0f)
    {
        Out.Diffuse = float4(0.0f,1.0f,0.0f,1.0f);
    }
    else
    {
        Out.Diffuse = Material.Diffuse;
    }
       
}

