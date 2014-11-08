/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
 */
/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma weak atan2 = __atan2

#include "libm.h"

/*
 * Let t(0) = 1 and for i = 1, ..., 160, let t(i) be the slope of
 * the line bisecting the conical hull of the set of points (x,y)
 * where x and y are positive normal floating point numbers and
 * the high order words hx and hy of their binary representations
 * satisfy |hx - hy - i * 0x8000| <= 0x4000.  Then:
 *
 * TBL[4*i+2] is t(i) rounded to 21 significant bits (i.e., the
 *   low order word is zero), and
 *
 * TBL[4*i] + TBL[4*i+1] form a doubled-double approximation to
 *   atan(TBL[4*i+2]).
 *
 * Finally, TBL[4*161] = TBL[4*161+1] = TBL[4*161+2] = 0.
 *
 * Now for any (x,y) with 0 < y <= x and any 0 < t <= 1,  we have
 * atan(y/x) = atan(t) + atan((y-t*x)/(x+t*y)).  By choosing t =
 * TBL[4*i+2], where i is the multiple of 0x8000 nearest hx - hy,
 * if this multiple is less than 161, and i = 161 otherwise, we
 * find that |(y-t*x)/(x+t*y)| <~ 2^-5.
 */
static const double TBL[] = {
	7.8539816339744827900e-01, +3.0616169978683830179e-17,
	1.0000000000000000000e+00, +0,
	7.7198905126506112140e-01, +2.6989956960083153652e-16,
	9.7353506088256835938e-01, +0,
	7.6068143954461309164e-01, -3.5178810518941914972e-16,
	9.5174932479858398438e-01, +0,
	7.4953661876353638860e-01, -3.2548100004524337476e-16,
	9.3073129653930664062e-01, +0,
	7.3854614984728339522e-01, -2.0775571023910406668e-16,
	9.1042709350585937500e-01, +0,
	7.2770146962041337702e-01, +3.8883249403168348802e-16,
	8.9078664779663085938e-01, +0,
	7.1699492488093774512e-01, -4.0468841511547224071e-16,
	8.7176513671875000000e-01, +0,
	7.0641813488653149022e-01, +5.6902424353981484031e-17,
	8.5331964492797851562e-01, +0,
	6.9596351101035658360e-01, +2.8245513321075021303e-16,
	8.3541154861450195312e-01, +0,
	6.8562363680534943455e-01, -4.2316970721658854064e-16,
	8.1800508499145507812e-01, +0,
	6.7539055666438230219e-01, +4.3535917281300047233e-16,
	8.0106592178344726562e-01, +0,
	6.6525763346931832132e-01, +1.1830431602404727977e-17,
	7.8456401824951171875e-01, +0,
	6.5521767574310185722e-01, -1.7435923100651044208e-16,
	7.6847028732299804688e-01, +0,
	6.4526390999481897381e-01, -1.4741927403093983947e-16,
	7.5275802612304687500e-01, +0,
	6.3538979894204850041e-01, +1.5734535069995660853e-16,
	7.3740243911743164062e-01, +0,
	6.2558914346942717799e-01, -2.8175588856316910960e-16,
	7.2238063812255859375e-01, +0,
	6.1585586476157949676e-01, -4.3056167357725226449e-16,
	7.0767116546630859375e-01, +0,
	6.0618408027576098362e-01, +1.5018013918429320289e-16,
	6.9325399398803710938e-01, +0,
	5.9656817827486730010e-01, +5.5271942033557644157e-17,
	6.7911052703857421875e-01, +0,
	5.8700289083426504533e-01, -8.2411369282676383293e-17,
	6.6522359848022460938e-01, +0,
	5.7748303053627658699e-01, +4.9400383775709159558e-17,
	6.5157699584960937500e-01, +0,
	5.6800353968303252117e-01, +2.9924431103311109543e-16,
	6.3815546035766601562e-01, +0,
	5.5855953863493823519e-01, -2.0306003403868777403e-16,
	6.2494468688964843750e-01, +0,
	5.4914706708329674711e-01, +2.8255378613779667461e-17,
	6.1193227767944335938e-01, +0,
	5.3976176660618069292e-01, +1.6370248781078747995e-16,
	5.9910583496093750000e-01, +0,
	5.3039888601412332747e-01, -7.6196097360093680134e-17,
	5.8645296096801757812e-01, +0,
	5.2105543924318808990e-01, -2.2400815668154739561e-16,
	5.7396411895751953125e-01, +0,
	5.1172778873967050828e-01, -3.6888136019899681185e-16,
	5.6162929534912109375e-01, +0,
	5.0241199666452196482e-01, -2.5412891474397011281e-16,
	5.4943847656250000000e-01, +0,
	4.9310493954293743712e-01, +4.4132186128251152229e-16,
	5.3738307952880859375e-01, +0,
	4.8380436844750995817e-01, -2.7844387907776656488e-16,
	5.2545595169067382812e-01, +0,
	4.7450670361463753721e-01, -2.0494355197368286028e-16,
	5.1364850997924804688e-01, +0,
	4.6367660027976320691e-01, +3.1709878607954760668e-16,
	5.0003623962402343750e-01, +0,
	4.5304753104003925301e-01, +3.3593436122420574865e-16,
	4.8681926727294921875e-01, +0,
	4.4423658037407065535e-01, +2.1987183192008082015e-17,
	4.7596645355224609375e-01, +0,
	4.3567016972500294258e-01, +3.0118422805369552650e-16,
	4.6550178527832031250e-01, +0,
	4.2733152672544871820e-01, -3.2667693224866479909e-16,
	4.5539522171020507812e-01, +0,
	4.1920540176693954493e-01, -2.2454273841113897647e-16,
	4.4561982154846191406e-01, +0,
	4.1127722812701872357e-01, -3.1620568973494653391e-16,
	4.3615055084228515625e-01, +0,
	4.0353384063084263289e-01, -3.5932009901481421723e-16,
	4.2696499824523925781e-01, +0,
	3.9596319345246833166e-01, -4.0281533417458698585e-16,
	4.1804289817810058594e-01, +0,
	3.8855405220339722661e-01, +1.6132231486045176674e-16,
	4.0936565399169921875e-01, +0,
	3.8129566313738116889e-01, +1.7684657060650804570e-16,
	4.0091586112976074219e-01, +0,
	3.7417884791401867517e-01, +2.6897604227426977619e-16,
	3.9267849922180175781e-01, +0,
	3.6719421967585041955e-01, -4.5886151448673745001e-17,
	3.8463878631591796875e-01, +0,
	3.6033388248727771241e-01, +1.5804115573136074946e-16,
	3.7678408622741699219e-01, +0,
	3.5358982224579182940e-01, +1.2624619863035782939e-16,
	3.6910200119018554688e-01, +0,
	3.4695498404186952968e-01, +9.3221684607372865177e-17,
	3.6158156394958496094e-01, +0,
	3.4042268308109679964e-01, +2.7697913559445449137e-16,
	3.5421252250671386719e-01, +0,
	3.3398684598563566084e-01, +3.6085337449716011085e-16,
	3.4698557853698730469e-01, +0,
	3.2764182824591436827e-01, +2.0581506352606456186e-16,
	3.3989214897155761719e-01, +0,
	3.2138200938788497041e-01, -1.9015787485430693661e-16,
	3.3292388916015625000e-01, +0,
	3.1520245348069497737e-01, +2.6961839659264087022e-16,
	3.2607340812683105469e-01, +0,
	3.0909871873117023000e-01, -1.5641891686756272625e-16,
	3.1933403015136718750e-01, +0,
	3.0306644308947827682e-01, +2.8801634211591956223e-16,
	3.1269931793212890625e-01, +0,
	2.9710135482774191473e-01, -4.3148994478973365819e-16,
	3.0616307258605957031e-01, +0,
	2.9120015759141004708e-01, -6.8539854790808585159e-17,
	2.9972028732299804688e-01, +0,
	2.8535879880370362827e-01, -1.2231638445300492682e-16,
	2.9336524009704589844e-01, +0,
	2.7957422506893880865e-01, -4.6707752931043135528e-17,
	2.8709340095520019531e-01, +0,
	2.7384352102802367313e-01, -4.1215636366229625876e-16,
	2.8090047836303710938e-01, +0,
	2.6816369484161040049e-01, -2.3700583122400495333e-16,
	2.7478218078613281250e-01, +0,
	2.6253212627627764419e-01, +2.3123213692190889610e-16,
	2.6873469352722167969e-01, +0,
	2.5694635355759309903e-01, -4.0638513814701264145e-16,
	2.6275444030761718750e-01, +0,
	2.5140385572454615470e-01, -3.4795333793554943723e-16,
	2.5683784484863281250e-01, +0,
	2.4500357070096612233e-01, +6.6542334848010259289e-17,
	2.5002646446228027344e-01, +0,
	2.3877766609573036760e-01, -2.7756633678549343650e-16,
	2.4342155456542968750e-01, +0,
	2.3365669377188336142e-01, +3.2700803838522067998e-16,
	2.3800384998321533203e-01, +0,
	2.2870810463931334766e-01, -4.4279127662219799521e-16,
	2.3278105258941650391e-01, +0,
	2.2391820542294382790e-01, +3.7558889374284208052e-16,
	2.2773718833923339844e-01, +0,
	2.1927501815429550902e-01, -1.4829838176513811186e-16,
	2.2285830974578857422e-01, +0,
	2.1476740847367459253e-01, -2.0535381496063397578e-17,
	2.1813154220581054688e-01, +0,
	2.1038568111737454558e-01, -4.2826767738736168650e-16,
	2.1354568004608154297e-01, +0,
	2.0612057974373865221e-01, +4.2108051749502232359e-16,
	2.0909011363983154297e-01, +0,
	2.0196410359405447821e-01, +3.5157118083511092869e-16,
	2.0475566387176513672e-01, +0,
	1.9790861144712756925e-01, +3.7894950972257700994e-16,
	2.0053362846374511719e-01, +0,
	1.9394752160084305359e-01, +2.8270367403478935534e-16,
	1.9641649723052978516e-01, +0,
	1.9007440763641536563e-01, -2.0842758095683676397e-16,
	1.9239699840545654297e-01, +0,
	1.8628369629742813629e-01, +3.4710917040399448932e-16,
	1.8846881389617919922e-01, +0,
	1.8256998712939509488e-01, +1.1053834120570125251e-16,
	1.8462586402893066406e-01, +0,
	1.7892875067284830237e-01, +3.0486232913366680305e-16,
	1.8086302280426025391e-01, +0,
	1.7535529778449010507e-01, -2.3810135019970148624e-16,
	1.7717504501342773438e-01, +0,
	1.7184559192514736736e-01, +5.1432582846210893916e-17,
	1.7355740070343017578e-01, +0,
	1.6839590847744290159e-01, +3.1605623296041433586e-18,
	1.7000591754913330078e-01, +0,
	1.6500283902547518977e-01, +1.5405422268770998251e-16,
	1.6651678085327148438e-01, +0,
	1.6166306303174859949e-01, +4.0042241517254928672e-16,
	1.6308629512786865234e-01, +0,
	1.5837358268281231943e-01, -2.2786616251622967291e-16,
	1.5971112251281738281e-01, +0,
	1.5513160990288810126e-01, -3.7547723514797166336e-16,
	1.5638816356658935547e-01, +0,
	1.5193468535499299321e-01, +4.3497510505554267446e-16,
	1.5311467647552490234e-01, +0,
	1.4878033155427861089e-01, -2.3102860235324261895e-16,
	1.4988791942596435547e-01, +0,
	1.4566628729590647140e-01, +9.9227592950040279415e-17,
	1.4670538902282714844e-01, +0,
	1.4259050967286590605e-01, -3.3869909683813096906e-18,
	1.4356482028961181641e-01, +0,
	1.3955105903633846509e-01, +1.5500435650773331566e-17,
	1.4046406745910644531e-01, +0,
	1.3654610022831903393e-01, +3.3965918616682805753e-16,
	1.3740110397338867188e-01, +0,
	1.3357402082462854764e-01, +2.7572431581527535421e-16,
	1.3437414169311523438e-01, +0,
	1.3063319828908959153e-01, -3.4667213797076707331e-16,
	1.3138139247894287109e-01, +0,
	1.2772200049776749609e-01, +3.1089261947725651968e-16,
	1.2842106819152832031e-01, +0,
	1.2436931430778752627e-01, -4.0654251891464630059e-16,
	1.2501454353332519531e-01, +0,
	1.2111683701666819957e-01, -3.9381654342464836012e-16,
	1.2171256542205810547e-01, +0,
	1.1844801833536511282e-01, -3.6673155595150283444e-16,
	1.1900508403778076172e-01, +0,
	1.1587365536613614125e-01, -1.5026628801318421951e-16,
	1.1639505624771118164e-01, +0,
	1.1338607085741525538e-01, +1.2886806274050538880e-16,
	1.1387449502944946289e-01, +0,
	1.1097844020819369604e-01, +2.3848343623577768044e-16,
	1.1143630743026733398e-01, +0,
	1.0864456107308662069e-01, +4.2065430313285469408e-16,
	1.0907405614852905273e-01, +0,
	1.0637891628473727934e-01, -4.6883543790348472687e-18,
	1.0678201913833618164e-01, +0,
	1.0417650062205296990e-01, +1.4774925414624453292e-16,
	1.0455501079559326172e-01, +0,
	1.0203276464730581807e-01, -1.5677032794816452332e-16,
	1.0238832235336303711e-01, +0,
	9.9943617083734892503e-02, +3.4511310907979792828e-16,
	1.0027772188186645508e-01, +0,
	9.7905249824711049200e-02, +3.4489485563461708496e-16,
	9.8219275474548339844e-02, +0,
	9.5914316649349906641e-02, -1.3214510886789011569e-17,
	9.6209526062011718750e-02, +0,
	9.3967698614664918466e-02, +1.1048427091217964090e-16,
	9.4245254993438720703e-02, +0,
	9.2062564267554769515e-02, -3.7297463814697759309e-16,
	9.2323541641235351562e-02, +0,
	9.0196252506350660383e-02, -3.5280143043576718079e-16,
	9.0441644191741943359e-02, +0,
	8.8366391663268650802e-02, -6.1140673227541621183e-17,
	8.8597118854522705078e-02, +0,
	8.6570782100201526532e-02, -2.0998844594957629702e-16,
	8.6787700653076171875e-02, +0,
	8.4807337678923566671e-02, +3.9530981588194673068e-16,
	8.5011243820190429688e-02, +0,
	8.3074323040850828193e-02, -4.3022503210464894539e-17,
	8.3265960216522216797e-02, +0,
	8.1369880712663267275e-02, -6.3063867569127169744e-18,
	8.1549942493438720703e-02, +0,
	7.9692445771216036121e-02, -5.0787623072962671502e-17,
	7.9861581325531005859e-02, +0,
	7.8040568735575632786e-02, -3.8810063021216721741e-16,
	7.8199386596679687500e-02, +0,
	7.6412797391314235540e-02, +4.1246529500495762995e-16,
	7.6561868190765380859e-02, +0,
	7.4807854772808823896e-02, -3.7025599052186724156e-16,
	7.4947714805603027344e-02, +0,
	7.3224639528778112663e-02, +4.2209138483206712401e-17,
	7.3355793952941894531e-02, +0,
	7.1661929761571485642e-02, -3.2074473649855177622e-16,
	7.1784853935241699219e-02, +0,
	7.0118738881148168218e-02, -2.5371257235753296804e-16,
	7.0233881473541259766e-02, +0,
	6.8594137996416115755e-02, +3.3796987842548399135e-16,
	6.8701922893524169922e-02, +0,
	6.7087137393172291411e-02, +5.5061492696328852397e-17,
	6.7187964916229248047e-02, +0,
	6.5596983299946565182e-02, -2.1580863111502565280e-16,
	6.5691232681274414062e-02, +0,
	6.4122802037412718335e-02, -3.1315661827469233434e-16,
	6.4210832118988037109e-02, +0,
	6.2426231582525915087e-02, -2.5758980071296622188e-16,
	6.2507450580596923828e-02, +0,
	6.0781559928021700046e-02, +1.3736899336217710591e-16,
	6.0856521129608154297e-02, +0,
	5.9432882624005145544e-02, +2.2246097394328856474e-16,
	5.9502959251403808594e-02, +0,
	5.8132551274581167888e-02, -6.2525053236379489390e-18,
	5.8198124170303344727e-02, +0,
	5.6876611930681164608e-02, -2.6589930995607417149e-16,
	5.6938022375106811523e-02, +0,
	5.5661522654748551986e-02, -4.2736362859832186197e-16,
	5.5719077587127685547e-02, +0,
	5.4484124463757943602e-02, -1.6708067365310384253e-16,
	5.4538100957870483398e-02, +0,
	5.3341582449436764080e-02, +3.3271673004611311850e-17,
	5.3392231464385986328e-02, +0,
	5.2231267345892007370e-02, -3.5593396674200571616e-16,
	5.2278816699981689453e-02, +0,
	5.1150874758829623090e-02, +1.4432815841187114832e-16,
	5.1195532083511352539e-02, +0,
	5.0098306612679444072e-02, +9.4680943793589404083e-17,
	5.0140261650085449219e-02, +0,
	4.9071641675614507960e-02, +2.1131168520301896817e-16,
	4.9111068248748779297e-02, +0,
	4.8069135772851545596e-02, +1.6035336741307516296e-16,
	4.8106193542480468750e-02, +0,
	4.7089192241088539959e-02, -2.2491738698796901479e-16,
	4.7124028205871582031e-02, +0,
	4.6130362086062248750e-02, -1.5111423469578965206e-16,
	4.6163111925125122070e-02, +0,
	4.5191314382707403752e-02, +4.1989325207399786612e-16,
	4.5222103595733642578e-02, +0,
	4.4270836390474244126e-02, -4.1432635292331004454e-16,
	4.4299781322479248047e-02, +0,
	4.3367774164955186222e-02, -3.0615383054587355892e-16,
	4.3394982814788818359e-02, +0,
	4.2481121875321825598e-02, -3.6730166956273555173e-16,
	4.2506694793701171875e-02, +0,
	4.1609902899457651415e-02, -4.4226425958068821782e-16,
	4.1633933782577514648e-02, +0,
	4.0753259129372665370e-02, +1.9801161516527046872e-16,
	4.0775835514068603516e-02, +0,
	3.9910361780060910064e-02, +8.2560620036613164573e-18,
	3.9931565523147583008e-02, +0,
	3.9080441183869218946e-02, +3.9908991939242971628e-17,
	3.9100348949432373047e-02, +0,
	3.8262816593271686827e-02, +9.5182237812195590276e-17,
	3.8281500339508056641e-02, +0,
	3.7456806948784837630e-02, +1.5213508760679563439e-16,
	3.7474334239959716797e-02, +0,
	3.6661849947035918262e-02, +7.3335516005184616486e-17,
	3.6678284406661987305e-02, +0,
	3.5877353272533163420e-02, -1.3007348019891714540e-16,
	3.5892754793167114258e-02, +0,
	3.5102754135096780885e-02, -2.9903662298950558656e-16,
	3.5117179155349731445e-02, +0,
	3.4337638360670830195e-02, +2.9656295131966114331e-16,
	3.4351140260696411133e-02, +0,
	3.3581472523789734907e-02, +3.4810947205572817820e-16,
	3.3594101667404174805e-02, +0,
	3.2833871859357266487e-02, -3.8885440174405159838e-16,
	3.2845675945281982422e-02, +0,
	3.2094421679560447558e-02, +5.8805134853032009978e-17,
	3.2105445861816406250e-02, +0,
	3.1243584858944295490e-02, +2.8737383773884313066e-17,
	3.1253755092620849609e-02, +0,
	0, 0, 0, 0
};

static const double C[] = {
	0.0,
	0.125,
	1.2980742146337069071e+33,
	7.8539816339744827900e-01,
	1.5707963267948965580e+00,
	6.1232339957367658860e-17,
	-3.1415926535897931160e+00,
	-1.2246467991473531772e-16,
	-3.33333333333327571893331786354179101074860633009e-0001,
	+1.99999999942671624230086497610394721817438631379e-0001,
	-1.42856965565428636896183013324727205980484158356e-0001,
	+1.10894981496317081405107718475040168084164825641e-0001,
};

#define	zero	C[0]
#define	twom3	C[1]
#define	two110	C[2]
#define	pio4	C[3]
#define	pio2	C[4]
#define	pio2_lo	C[5]
#define	mpi	C[6]
#define	mpi_lo	C[7]
#define	p1	C[8]
#define	p2	C[9]
#define	p3	C[10]
#define	p4	C[11]

double
atan2(double oy, double ox) {
	double	ah, al, t, xh, x, y, z;
	int	i, k, hx, hy, sx, sy;
#ifndef lint
	volatile int	inexact;
#endif

	hy = ((int *)&oy)[HIWORD];
	sy = hy & 0x80000000;
	hy &= ~0x80000000;

	hx = ((int *)&ox)[HIWORD];
	sx = hx & 0x80000000;
	hx &= ~0x80000000;

	if (hy > hx || (hy == hx && ((unsigned *)&oy)[LOWORD] >
	    ((unsigned *)&ox)[LOWORD])) {
		i = hx;
		hx = hy;
		hy = i;
		x = fabs(oy);
		y = fabs(ox);
		if (sx) {
			ah = pio2;
			al = pio2_lo;
		} else {
			ah = -pio2;
			al = -pio2_lo;
			sy ^= 0x80000000;
		}
	} else {
		x = fabs(ox);
		y = fabs(oy);
		if (sx) {
			ah = mpi;
			al = mpi_lo;
			sy ^= 0x80000000;
		} else {
			ah = al = zero;
		}
	}

	if (hx >= 0x7fe00000 || hx - hy >= 0x03600000) {
		if (hx >= 0x7ff00000) {
			if (((hx ^ 0x7ff00000) | ((int *)&x)[LOWORD]) != 0)
				return (ox * oy);
			if (hy >= 0x7ff00000)
				ah += pio4;
#ifndef lint
			inexact = (int)ah;	/* inexact if ah != 0 */
#endif
			return ((sy)? -ah : ah);
		}
		if (hx - hy >= 0x03600000) {
			if ((int)ah == 0)
				ah = y / x;
			return ((sy)? -ah : ah);
		}
		y *= twom3;
		x *= twom3;
		hy -= 0x00300000;
		hx -= 0x00300000;
	} else if (hy < 0x00100000) {
		if ((hy | ((int *)&y)[LOWORD]) == 0) {
			if ((hx | ((int *)&x)[LOWORD]) == 0)
				return (_SVID_libm_err(ox, oy, 3));
#ifndef lint
			inexact = (int)ah;	/* inexact if ah != 0 */
#endif
			return ((sy)? -ah : ah);
		}
		y *= two110;
		x *= two110;
		hy = ((int *)&y)[HIWORD];
		hx = ((int *)&x)[HIWORD];
	}

	k = (((hx - hy) + 0x00004000) >> 13) & ~0x3;
	if (k > 644)
		k = 644;
	ah += TBL[k];
	al += TBL[k+1];
	t = TBL[k+2];

	xh = x;
	((int *)&xh)[LOWORD] = 0;
	z = ((y - t * xh) - t * (x - xh)) / (x + y * t);
	x = z * z;
	t = ah + (z + (al + (z * x) * (p1 + x * (p2 + x * (p3 + x * p4)))));
	return ((sy)? -t : t);
}
