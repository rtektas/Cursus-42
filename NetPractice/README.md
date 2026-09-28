*This project has been created as part of the 42 curriculum by rtektas.*

# NetPractice

## Description

NetPractice is a networking training project from the 42 curriculum.  
The goal of this project is to understand and practice fundamental networking concepts by configuring virtual networks across multiple levels.

Each level presents a network topology composed of hosts, routers, switches, and the Internet. The objective is to correctly configure IP addresses, subnet masks, and routing tables so that all required communications between devices succeed.

Through this project, students learn how data flows through networks and how routing decisions are made based on network configuration.

---

## Instructions

### Running NetPractice

1. Open the NetPractice training interface:
   - Launch `index.html` in a web browser (Firefox or Chromium recommended).
2. Select the desired level.
3. Configure:
   - IP addresses
   - Subnet masks
   - Routing tables
4. Use the **Check** button to validate each level.

### Exporting configurations

- Once a level is validated, export the configuration using the **Export** button.
- This generates a `.json` file containing the full configuration for that level.

### Submission requirements

- **10 exported configuration files** (one per level) must be placed at the **root of the Git repository**.
- The repository must also contain this `README.md` file at the root.

---

## Resources

### Networking concepts studied

This project covers the following networking concepts:

- IP addressing
- IPv4 addresses
- Subnet masks (`/24`, `/26`, `/27`, `/30`, etc.)
- CIDR notation
- Default gateway
- Routing tables
- Routers and switches
- Local Area Networks (LAN)
- Wide Area Networks (WAN)
- Public vs private IP addresses
- OSI model layers (mainly Layer 2 and Layer 3)
- Forward path and reverse path routing

### References

- https://en.wikipedia.org/wiki/Subnetwork
- https://en.wikipedia.org/wiki/Routing_table
- https://en.wikipedia.org/wiki/OSI_model
- https://www.youtube.com/watch?v=HQUw0CfQWAM&t=1097s

### Use of Artificial Intelligence

AI tools were used during this project as a **learning aid** to:
- Understand networking theory (subnetting, routing logic)
- Analyze routing errors such as *No forward way* and *No reverse way*
- Validate network design logic
- Improve explanations and reasoning about IP addressing and routing behavior

All configurations and final decisions were implemented and validated manually by the student.

---

## Notes

NetPractice is a logic-based project: the exact IP values are less important than understanding how networks are structured and how routing decisions are made. The project emphasizes reasoning, consistency, and correct use of networking principles.
