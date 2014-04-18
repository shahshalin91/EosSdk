// Copyright (c) 2014 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef EOS_TYPES_NEXTHOP_GROUP_H
#define EOS_TYPES_NEXTHOP_GROUP_H

#include <eos/panic.h>
#include <eos/utility.h>
#include <forward_list>
#include <map>
#include <sstream>

namespace eos {

/**
 * The type of encapsulation to use for this nexthop group.

 * The present nexthop group types are all tunnel encapsulations. Each
 * causes a variety of packet headers for packets using the group to
 * be changed to appropriately encapsulate the frame.
 */
enum nexthop_group_encap_t {
   NEXTHOP_GROUP_TYPE_NULL,
   /** IP in IP encapsulation. */
   NEXTHOP_GROUP_IP_IN_IP,
   /** GRE encapsulation. */
   NEXTHOP_GROUP_GRE,
   /** MPLS encapsulation. */
   NEXTHOP_GROUP_MPLS,
};


/**
 * An MPLS nexthop group switching operation.

 * This structure combines a stack of labels and an MPLS switchingoperation using
 * those labels, such as eos::MPLS_ACTION_PUSH.
 */
class EOS_SDK_PUBLIC nexthop_group_mpls_action_t {
 public:
   /** Default constructor. */
   nexthop_group_mpls_action_t();
   /** Constructs an MPLS action with a specific switching operation. */
   explicit nexthop_group_mpls_action_t(mpls_action_t action_type);

   /** Getter for 'action_type': The MPLS switching operation for this action. */
   mpls_action_t action_type() const;
   /** Setter for 'action_type'. */
   void action_type_is(mpls_action_t action_type);

   /**
    * Getter for 'label_stack': The MPLS label stack.

    * The first element is the outermost label.
    */
   std::forward_list<mpls_label_t> const & label_stack() const;
   /** Setter for 'label_stack'. */
   void label_stack_is(std::forward_list<mpls_label_t> const & label_stack);
   /** Prepend one label_stack to the list. */
   void label_stack_set(mpls_label_t const & label_stack);
   /** Remove all matching label_stack elements. */
   void label_stack_del(mpls_label_t const & label_stack);

   bool operator==(nexthop_group_mpls_action_t const & other) const;
   bool operator!=(nexthop_group_mpls_action_t const & other) const;
   bool operator<(nexthop_group_mpls_action_t const & other) const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of
    * nexthop_group_mpls_action_t to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os, 
                                   const nexthop_group_mpls_action_t& obj);

 private:
   mpls_action_t action_type_;
   std::forward_list<mpls_label_t> label_stack_;
};


/**
 * A nexthop group destination entry.

 * An entry consists of a nexthop IP address, and optionally an MPLS
 * label switching operation.
 */
class EOS_SDK_PUBLIC nexthop_group_entry_t {
 public:
   nexthop_group_entry_t();
   explicit nexthop_group_entry_t(ip_addr_t const & nexthop);

   /** Getter for 'mpls_action': MPLS label switching stack for this entry. */
   nexthop_group_mpls_action_t mpls_action() const;
   /** Setter for 'mpls_action'. */
   void mpls_action_is(nexthop_group_mpls_action_t const & mpls_action);

   /** Getter for 'nexthop': The next hop IP address for this entry. */
   ip_addr_t nexthop() const;
   /** Setter for 'nexthop'. */
   void nexthop_is(ip_addr_t const & nexthop);

   bool operator==(nexthop_group_entry_t const & other) const;
   bool operator!=(nexthop_group_entry_t const & other) const;
   bool operator<(nexthop_group_entry_t const & other) const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of
    * nexthop_group_entry_t to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os, 
                                   const nexthop_group_entry_t& obj);

 private:
   nexthop_group_mpls_action_t mpls_action_;
   ip_addr_t nexthop_;
};


/**
 * A nexthop group.

 * A nexthop group represents encapsulation and IP addressing
 * information to be used with a policy routing application.
 */
class EOS_SDK_PUBLIC nexthop_group_t {
 public:
   nexthop_group_t();
   nexthop_group_t(std::string name, nexthop_group_encap_t type);

   /** Getter for 'name': The unique name of the nexthop group. */
   std::string name() const;

   /** Getter for 'type': The type of packet encapsulation used on the group. */
   nexthop_group_encap_t type() const;

   /** Getter for 'ttl': The TTL set in frame headers of IP-in-IP or GRE tunnels. */
   uint16_t ttl() const;
   /** Setter for 'ttl'. */
   void ttl_is(uint16_t ttl);

   /** Getter for 'source_ip': The source IP used on frames sent on this group. */
   ip_addr_t source_ip() const;
   /** Setter for 'source_ip'. */
   void source_ip_is(ip_addr_t const & source_ip);

   /** Getter for 'source_intf': The source interface to use. */
   intf_id_t source_intf() const;
   /** Setter for 'source_intf'. */
   void source_intf_is(intf_id_t const & source_intf);

   uint16_t size() const;
   /** Getter for 'nexthops': Array index to nexthop group entry map. */
   std::map<uint16_t, nexthop_group_entry_t> const & nexthops() const;
   /** Setter for 'nexthops'. */
   void nexthops_is(std::map<uint16_t, nexthop_group_entry_t> const & nexthops);
   /** inserts key/value pair to the map. */
   void nexthop_set(uint16_t key, nexthop_group_entry_t const & value);
   /** deletes the key/value pair from the map. */
   void nexthop_del(uint16_t key);

   /**
    * Getter for 'destination_ips': Array index to IP address map.
    * @deprecated Use 'nexthops' instead.
    */
   std::map<uint16_t, ip_addr_t> const & destination_ips() const;
   /** Setter for 'destination_ips'. */
   void destination_ips_is(
                           std::map<uint16_t, ip_addr_t> const & destination_ips);
   /** inserts key/value pair to the map. */
   void destination_ip_set(uint16_t key, ip_addr_t const & value);
   /** deletes the key/value pair from the map. */
   void destination_ip_del(uint16_t key);

   /** Getter for 'persistent': The source interface to use. */
   bool persistent() const;
   /** Setter for 'persistent'. */
   void persistent_is(bool persistent);

   bool operator==(nexthop_group_t const & other) const;
   bool operator!=(nexthop_group_t const & other) const;
   bool operator<(nexthop_group_t const & other) const;
   /** Returns a string representation of the current object's values. */
   std::string to_string() const;
   /**
    * A utility stream operator that adds a string representation of
    * nexthop_group_t to the ostream.
    */
   friend std::ostream& operator<<(std::ostream& os, const nexthop_group_t& obj);

 private:
   std::string name_;
   nexthop_group_encap_t type_;
   uint16_t ttl_;
   ip_addr_t source_ip_;
   intf_id_t source_intf_;
   uint8_t size_;
   std::map<uint16_t, nexthop_group_entry_t> nexthops_;
   std::map<uint16_t, ip_addr_t> destination_ips_;
   bool persistent_;
};

}

#include <eos/inline/types/nexthop_group.h>

#endif // EOS_TYPES_NEXTHOP_GROUP_H