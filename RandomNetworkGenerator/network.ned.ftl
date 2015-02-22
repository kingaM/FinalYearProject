<#assign gateName = "gate">
import Channel;
import Node;
import SourceNode;
import SinkNode;
import EvilNode;

<#assign neighborLists = topo.generate()>
<#assign sinks = topo.getSinks(1)>
<#assign attackers = topo.getAttackers(1)>
<#assign sources = topo.getSources(1)> 
<#assign index = numNodes> 

//
// Generated network with random topology (${numNodes} nodes, ${numLinks} edges, seed=${seed}).
//

network ${targetTypeName} {
    submodules:
<#list 0..(neighborLists?size-1) as i>
        node${i} : <#compress><#if attackers?seq_contains(i)>
              EvilNode
        			<#elseif sinks?seq_contains(i)>
              SinkNode
        			<#elseif sources?seq_contains(i)>
              SourceNode
        			<#else>
              Node
              </#if> 
              </#compress>
 {
          id = ${i};
        };
</#list>
    connections:
<#list 0..neighborLists?size-1 as i>
  <#assign neighborList = neighborLists[i] >
  <#list neighborList as neighbor>
     <#if (i < neighbor)>
        node${i}.${gateName}++ <--> Channel {
          id = ${index};
          <#assign index = index + 1> 
        } <--> node${neighbor}.${gateName}++;
     </#if>
  </#list>
</#list>
}
