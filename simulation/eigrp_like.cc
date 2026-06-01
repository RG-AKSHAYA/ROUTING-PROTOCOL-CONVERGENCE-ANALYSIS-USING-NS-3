
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("EigrpLike");

void FailLink (Ptr<PointToPointChannel> channel)
{
    NS_LOG_INFO ("Link failure injected at t=" << Simulator::Now ().GetSeconds () << "s");
    channel->SetAttribute ("Delay", StringValue ("1000s"));
}

void RecoverLink (Ptr<PointToPointChannel> channel)
{
    NS_LOG_INFO ("Link recovered at t=" << Simulator::Now ().GetSeconds () << "s");
    channel->SetAttribute ("Delay", StringValue ("2ms"));
}

int main (int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse (argc, argv);

    NodeContainer routers;
    routers.Create (6);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute  ("DataRate", StringValue ("5Mbps"));
    p2p.SetChannelAttribute ("Delay",    StringValue ("2ms"));

    NetDeviceContainer d01 = p2p.Install (routers.Get (0), routers.Get (1));
    NetDeviceContainer d12 = p2p.Install (routers.Get (1), routers.Get (2));
    NetDeviceContainer d23 = p2p.Install (routers.Get (2), routers.Get (3));
    NetDeviceContainer d34 = p2p.Install (routers.Get (3), routers.Get (4));
    NetDeviceContainer d45 = p2p.Install (routers.Get (4), routers.Get (5));

    InternetStackHelper internet;
    internet.Install (routers);

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0"); address.Assign (d01);
    address.SetBase ("10.1.2.0", "255.255.255.0"); address.Assign (d12);
    address.SetBase ("10.1.3.0", "255.255.255.0"); address.Assign (d23);
    address.SetBase ("10.1.4.0", "255.255.255.0"); address.Assign (d34);
    address.SetBase ("10.1.5.0", "255.255.255.0"); address.Assign (d45);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    Ptr<PointToPointChannel> broken =
        DynamicCast<PointToPointChannel> (d23.Get (0)->GetChannel ());
    Simulator::Schedule (Seconds (8.0),  &FailLink,    broken);
    Simulator::Schedule (Seconds (12.0), &RecoverLink, broken);
    Simulator::Schedule (Seconds (9.0),  &Ipv4GlobalRoutingHelper::RecomputeRoutingTables);
    Simulator::Schedule (Seconds (13.0), &Ipv4GlobalRoutingHelper::RecomputeRoutingTables);

    Ipv4GlobalRoutingHelper g;
    Ptr<OutputStreamWrapper> routingStream =
        Create<OutputStreamWrapper> ("eigrp_routes.txt", std::ios::out);
    g.PrintRoutingTableAllAt (Seconds (5.0),  routingStream);
    g.PrintRoutingTableAllAt (Seconds (10.0), routingStream);
    g.PrintRoutingTableAllAt (Seconds (14.0), routingStream);

    p2p.EnablePcapAll ("eigrp_like");
    AnimationInterface anim ("eigrp_animation.xml");

    Simulator::Stop    (Seconds (20.0));
    Simulator::Run     ();
    Simulator::Destroy ();

    return 0;
}
