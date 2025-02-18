---
page_type: sample
languages:
- c
products:
- azure
- azure-sphere
name: Azure Sphere – DNS service discovery
urlFragment: DNSServiceDiscovery
extendedZipContent:
- path: .clang-format
  target: .clang-format
- path: BUILD_INSTRUCTIONS.md
  target: BUILD_INSTRUCTIONS.md
- path: Samples/SECURITY.md
  target: SECURITY.md
- path: Samples/troubleshooting.md
  target: troubleshooting.md
description: "Demonstrates how to perform service discovery on the local network by using multicast DNS (mDNS)."
---

# Sample: DNS service discovery

This sample demonstrates how to perform [DNS service discovery](https://learn.microsoft.com/azure-sphere/app-development/service-discovery) by sending DNS-SD queries to the local network using multicast DNS (mDNS).

The application queries the local network for **PTR** records that identify all instances of the _sample-service._tcp service. The application then queries the network for the **SRV**, **TXT**, and **A** records that contain the DNS details for each service instance. After service discovery is performed, the Azure Sphere firewall allows the application to connect to the discovered host names.

The sample uses the following Azure Sphere libraries.

| Library | Purpose |
|---------|---------|
| [eventloop](https://learn.microsoft.com/azure-sphere/reference/applibs-reference/applibs-eventloop/eventloop-overview) | Invokes handlers for timer events. |
| [log](https://learn.microsoft.com/azure-sphere/reference/applibs-reference/applibs-log/log-overview) | Displays messages in the **Device Output** window during debugging. |
| [networking](https://learn.microsoft.com/azure-sphere/reference/applibs-reference/applibs-networking/networking-overview) | Manages network connectivity. |

## Contents

| File/folder           | Description |
|-----------------------|-------------|
| `app_manifest.json`   | Application manifest file, which describes the resources. |
| `CMakeLists.txt`      | CMake configuration file, which Contains the project information and is required for all builds. |
| `CMakePresets.json`   | CMake presets file, which contains the information to configure the CMake project. |
| `launch.vs.json`      | JSON file that tells Visual Studio how to deploy and debug the application. |
| `LICENSE.txt`         | The license for this sample application. |
| `main.c`              | Main C source code file. |
| `README.md`           | This README file. |
| `.vscode`             | Folder containing the JSON files that configure Visual Studio Code for deploying and debugging the application. |

## Prerequisites

The sample requires the following hardware:

- An [Azure Sphere development board](https://aka.ms/azurespheredevkits)

## Setup

1. Even if you've performed this set up previously, ensure that you have Azure Sphere SDK version 22.11 or above. At the command prompt, run **azsphere show-version** to check. Install [the Azure Sphere SDK](https://learn.microsoft.com/azure-sphere/install/install-sdk) as needed.
1. Connect your Azure Sphere device to your computer by USB.
1. Connect your Azure Sphere device to the same local network as the DNS service.
1. Enable application development, if you have not already done so, by entering the **azsphere device enable-development** command at the command prompt.
1. Clone the [Azure Sphere samples](https://github.com/Azure/azure-sphere-samples) repository and find the *DNSServiceDiscovery* sample in the *DNSServiceDiscovery* folder or download the zip file from the [Microsoft samples browser](https://learn.microsoft.com/samples/azure/azure-sphere-samples/dnsservicediscovery/).

1. Set up a DNS service. This sample requires that you run a DNS service instance that is discoverable on the same local network as the Azure Sphere device. You can use the DNS-SD tool from [Apple Bonjour](https://developer.apple.com/bonjour/) to set up the service.

    The following **dns-sd** command registers an instance of a DNS responder service with the default service configuration used by the sample:

    ```
    dns-sd -R SampleInstanceName _sample-service._tcp local 1234 SampleTxtData
    ```

    The command registers a service instance with the following configuration:

    - service instance name: SampleInstanceName
    - DNS server type: _sample-service._tcp
    - DNS server domain: local
    - port: 1234
    - TXT record: SampleTxtData

1. You may want to modify the sample to use unicast queries if you don't need to use multicast queries. You can use unicast queries by calling the **res_send** POSIX API to query the DNS server and process the response in a single blocking call. This may simplify the application, especially if it doesn't need to perform other activities while waiting for the response.

### Use Ethernet instead of Wi-Fi

By default, this sample runs over a Wi-Fi connection to the internet. To use Ethernet instead, complete the following steps:

1. Follow the [Ethernet setup instructions](https://learn.microsoft.com/azure-sphere/network/connect-ethernet).
1. Ensure that the global constant **networkInterface** is set to "eth0". Find the following line of code in `main.c` and  replace `wlan0` with `eth0`:

    ```c
    char networkInterface[] = "wlan0";
    ```

## Build and run the sample

To build and run this sample, follow the instructions in [Build a sample application](../../BUILD_INSTRUCTIONS.md).

### Test the sample

When you run the sample, every 10 seconds the application will send a DNS query. When it receives a response, it displays the name, host, IPv4 address, port, and TXT data from the query response. The application should then be able to connect to the host names returned by the response.

You can verify the connection by setting up a local web server on the same computer as the DNS service, and then making requests to the service from the application.

To set up an Internet Information Services (IIS) web server, complete the following steps:

1. Install [IIS](https://www.iis.net/) on the same computer as the DNS service.
1. If you set up a site binding for a default website with a port other than 80 or 443, you must add an inbound rule that allows the port.

To send requests to the web server, you can incorporate code from the [HTTPS_Curl_Easy](https://github.com/Azure/azure-sphere-samples/tree/main/Samples/HTTPS/HTTPS_Curl_Easy) sample into the application. Requests to the web server should fail before the DNS-SD responses are received but should succeed afterwards.

## Rebuild the sample to query a different DNS server

By default, this sample queries the _sample-service._tcp.local DNS server address. To rebuild the sample to query a different DNS server, complete the following steps.

1. Make the following changes in the sample:

    1. In the `app_manifest.json` file, change the value of the **AllowedConnections** field from `"_sample-service._tcp.local"` to the new DNS server address, such as `"_http._tcp.local"`.
    1. In `main.c`, find the following line of code and replace `_sample-service._tcp.local` with the new DNS server address:

        ```c
        static const char DnsServiceDiscoveryServer[] = "_sample-service._tcp.local";
        ```

1. Follow the instructions in the [Build and run the sample](#build-and-run-the-sample) section of this README.

## Next steps

- For an overview of Azure Sphere, see [What is Azure Sphere](https://learn.microsoft.com/azure-sphere/product-overview/what-is-azure-sphere).
- To learn more about Azure Sphere application development, see [Overview of Azure Sphere applications](https://learn.microsoft.com/azure-sphere/app-development/applications-overview).
- For network troubleshooting, see [Troubleshoot network problems](https://learn.microsoft.com/azure-sphere/network/troubleshoot-network-problems).
