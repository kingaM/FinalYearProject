<@setoutput path=targetMainFile?default("")/>
${bannerComment}

<#if nedPackageName!="">package ${nedPackageName};</#if>
<#if gateName==""><#assign gateName = "gate"></#if>
<#assign gateName = StringUtils.makeValidIdentifier(gateName)>

import Channel;

<#if channelTypeSupplied>
  <#assign channelSpec = " " + channelType + " <-->">
<#else>
  <#assign channelSpec = "">
</#if>

<#if !nodeTypeExists>
module ${nodeType} {
    parameters:
        @display("i=abstract/router_vs");
    gates:
        inout ${gateName}[];
    connections allowunconnected:
}
</#if>

<#assign numNodes = numNodes?number>
<#assign numLinks = numLinks?number>
<#assign seed = seed?number>
<#assign param1 = param1Times100?number / 100.0>
<#assign param2 = param2Times100?number / 100.0>

<#assign topo = LangUtils.newInstance("Topogen")>
<@do topo.setNodes(numNodes) !/>
<@do topo.setEdges(numLinks) !/>
<@do topo.setSeed(seed) !/>
<@do topo.setParam1(param1) !/>
<@do topo.setParam2(param2) !/>
<#assign neighborLists = topo.generate()>
<#assign sinks = topo.getSinks(1)>
<#assign attackers = topo.getAttackers(1)>
<#assign sources = topo.getSources(1)>

//
// Generated network with random topology (${numNodes} nodes, ${numLinks} edges, seed=${seed}).
//
<#assign networkKeyword = iif(wizardType=="compoundmodule", "module", "network")>
${networkKeyword} ${targetTypeName} {
    submodules:
<#list 0..neighborLists.size()-1 as i>
        node${i} : <#if attackers.contains(i)>EvilNode;
        			<#elseif sinks.contains(i)>SinkNode;
        			<#elseif sources.contains(i)>SourceNode;
        			<#else>Node;</#if>
</#list>
    connections:
<#list 0..neighborLists.size()-1 as i>
  <#assign neighborList = neighborLists[i] >
  <#list neighborList as neighbor>
     <#if (i < neighbor)>
        node${i}.${gateName}++ <-->${channelSpec} node${neighbor}.${gateName}++;
     </#if>
  </#list>
</#list>
}
