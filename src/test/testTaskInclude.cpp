#include <pps/pps.h>
#include <iostream>

int main()
{
    pps::DefineCTX conditions;
    conditions.bools = {
        {"@useBaseColorMap", true},
        {"@useBaseColorAlpha", true},
        {"@isRaster", true},
        {"@useShadow", false},
    };

    pps::ReplaceCTX replace;
    replace.texts = {
        {"@useBaseColorMap", "mat.useBaseColorMap"},
        {"@useBaseColorAlpha", "mat.useBaseColorAlpha"},
        {"@useShadow", "scene.useShadow"},
    };

    pps::IncludeCTX include;
    include.prefixes = {
        std::string(SOURCE_DIR) + "/include/",
    };

    pps::PPS lang(conditions, replace, include);
    std::string line = R"(
/*<$include pps/pps.h>*/

/*<$branch if @useBaseColorMap>*/
{
    float4 value = baseColorMap(...);
    color.rgb *= value.rgb;
    /*<$branch if @useBaseColorAlpha>*/
    color.a *= value.a;
    /*<$branch endif>*/
}
/*<$branch endif>*/
/*<$branch if @isRaster && @useShadow>*/
{
    color *= shadow(...);   
}
/*<$branch endif>*/
)";
    auto result = lang.process(line, false);

    std::cout << "pps result:\n"
              << result << std::endl;
}
