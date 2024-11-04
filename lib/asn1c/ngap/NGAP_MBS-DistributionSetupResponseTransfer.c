/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "../support/ngap-r17.3.0/38413-h30.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER -no-gen-JER`
 */

#include "NGAP_MBS-DistributionSetupResponseTransfer.h"

#include "NGAP_MBS-SessionTNLInfo5GCItem.h"
#include "NGAP_MBS-ServiceArea.h"
#include "NGAP_ProtocolExtensionContainer.h"
static asn_TYPE_member_t asn_MBR_NGAP_MBS_DistributionSetupResponseTransfer_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, mBS_SessionID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NGAP_MBS_SessionID,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"mBS-SessionID"
		},
	{ ATF_POINTER, 2, offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, mBS_AreaSessionID),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NGAP_MBS_AreaSessionID,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"mBS-AreaSessionID"
		},
	{ ATF_POINTER, 1, offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, sharedNGU_MulticastTNLInformation),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NGAP_MBS_SessionTNLInfo5GCItem,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"sharedNGU-MulticastTNLInformation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, mBS_QoSFlowsToBeSetupList),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NGAP_MBS_QoSFlowsToBeSetupList,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"mBS-QoSFlowsToBeSetupList"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, mBSSessionStatus),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NGAP_MBSSessionStatus,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"mBSSessionStatus"
		},
	{ ATF_POINTER, 2, offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, mBS_ServiceArea),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_NGAP_MBS_ServiceArea,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"mBS-ServiceArea"
		},
	{ ATF_POINTER, 1, offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NGAP_ProtocolExtensionContainer_11905P167,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_NGAP_MBS_DistributionSetupResponseTransfer_oms_1[] = { 1, 2, 5, 6 };
static const ber_tlv_tag_t asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_NGAP_MBS_DistributionSetupResponseTransfer_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mBS-SessionID */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* mBS-AreaSessionID */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* sharedNGU-MulticastTNLInformation */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* mBS-QoSFlowsToBeSetupList */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* mBSSessionStatus */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* mBS-ServiceArea */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* iE-Extensions */
};
static asn_SEQUENCE_specifics_t asn_SPC_NGAP_MBS_DistributionSetupResponseTransfer_specs_1 = {
	sizeof(struct NGAP_MBS_DistributionSetupResponseTransfer),
	offsetof(struct NGAP_MBS_DistributionSetupResponseTransfer, _asn_ctx),
	asn_MAP_NGAP_MBS_DistributionSetupResponseTransfer_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_NGAP_MBS_DistributionSetupResponseTransfer_oms_1,	/* Optional members */
	4, 0,	/* Root/Additions */
	7,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer = {
	"MBS-DistributionSetupResponseTransfer",
	"MBS-DistributionSetupResponseTransfer",
	&asn_OP_SEQUENCE,
	asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer_tags_1,
	sizeof(asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer_tags_1)
		/sizeof(asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer_tags_1[0]), /* 1 */
	asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer_tags_1,	/* Same as above */
	sizeof(asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer_tags_1)
		/sizeof(asn_DEF_NGAP_MBS_DistributionSetupResponseTransfer_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_NGAP_MBS_DistributionSetupResponseTransfer_1,
	7,	/* Elements count */
	&asn_SPC_NGAP_MBS_DistributionSetupResponseTransfer_specs_1	/* Additional specs */
};

