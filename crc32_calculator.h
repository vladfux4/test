#ifndef CRC32_CALCULATOR_H_
#define CRC32_CALCULATOR_H_

#include <stdint.h>

template<uint32_t Rate>
struct CRC32Calculator {
  CRC32Calculator() = delete;

  static uint32_t Calculate(const uint8_t *buf, uint32_t len, const uint32_t init) {
    uint32_t crc = init;
    while (len--) {
      crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *buf) & 255];
      buf++;
    }

    return crc;
  }

 private:
  constexpr static uint32_t CalcRateRec(uint32_t c, uint32_t j) {
    if (j > 0) {
      j--;
      c = c & 0x80000000 ? (c << 1) ^ Rate : (c << 1);
      return CalcRateRec(c, j);
    }

    return c;
  }

  constexpr static uint32_t CalcRate(const uint32_t index) {
    return CalcRateRec((index << 24), 8);
  }

  static constexpr uint32_t crc32_table[] = {
    CalcRate(0), CalcRate(1), CalcRate(2), CalcRate(3),
    CalcRate(4), CalcRate(5), CalcRate(6), CalcRate(7),
    CalcRate(8), CalcRate(9), CalcRate(10), CalcRate(11),
    CalcRate(12), CalcRate(13), CalcRate(14), CalcRate(15),
    CalcRate(16), CalcRate(17), CalcRate(18), CalcRate(19),
    CalcRate(20), CalcRate(21), CalcRate(22), CalcRate(23),
    CalcRate(24), CalcRate(25), CalcRate(26), CalcRate(27),
    CalcRate(28), CalcRate(29), CalcRate(30), CalcRate(31),
    CalcRate(32), CalcRate(33), CalcRate(34), CalcRate(35),
    CalcRate(36), CalcRate(37), CalcRate(38), CalcRate(39),
    CalcRate(40), CalcRate(41), CalcRate(42), CalcRate(43),
    CalcRate(44), CalcRate(45), CalcRate(46), CalcRate(47),
    CalcRate(48), CalcRate(49), CalcRate(50), CalcRate(51),
    CalcRate(52), CalcRate(53), CalcRate(54), CalcRate(55),
    CalcRate(56), CalcRate(57), CalcRate(58), CalcRate(59),
    CalcRate(60), CalcRate(61), CalcRate(62), CalcRate(63),
    CalcRate(64), CalcRate(65), CalcRate(66), CalcRate(67),
    CalcRate(68), CalcRate(69), CalcRate(70), CalcRate(71),
    CalcRate(72), CalcRate(73), CalcRate(74), CalcRate(75),
    CalcRate(76), CalcRate(77), CalcRate(78), CalcRate(79),
    CalcRate(80), CalcRate(81), CalcRate(82), CalcRate(83),
    CalcRate(84), CalcRate(85), CalcRate(86), CalcRate(87),
    CalcRate(88), CalcRate(89), CalcRate(90), CalcRate(91),
    CalcRate(92), CalcRate(93), CalcRate(94), CalcRate(95),
    CalcRate(96), CalcRate(97), CalcRate(98), CalcRate(99),
    CalcRate(100), CalcRate(101), CalcRate(102), CalcRate(103),
    CalcRate(104), CalcRate(105), CalcRate(106), CalcRate(107),
    CalcRate(108), CalcRate(109), CalcRate(110), CalcRate(111),
    CalcRate(112), CalcRate(113), CalcRate(114), CalcRate(115),
    CalcRate(116), CalcRate(117), CalcRate(118), CalcRate(119),
    CalcRate(120), CalcRate(121), CalcRate(122), CalcRate(123),
    CalcRate(124), CalcRate(125), CalcRate(126), CalcRate(127),
    CalcRate(128), CalcRate(129), CalcRate(130), CalcRate(131),
    CalcRate(132), CalcRate(133), CalcRate(134), CalcRate(135),
    CalcRate(136), CalcRate(137), CalcRate(138), CalcRate(139),
    CalcRate(140), CalcRate(141), CalcRate(142), CalcRate(143),
    CalcRate(144), CalcRate(145), CalcRate(146), CalcRate(147),
    CalcRate(148), CalcRate(149), CalcRate(150), CalcRate(151),
    CalcRate(152), CalcRate(153), CalcRate(154), CalcRate(155),
    CalcRate(156), CalcRate(157), CalcRate(158), CalcRate(159),
    CalcRate(160), CalcRate(161), CalcRate(162), CalcRate(163),
    CalcRate(164), CalcRate(165), CalcRate(166), CalcRate(167),
    CalcRate(168), CalcRate(169), CalcRate(170), CalcRate(171),
    CalcRate(172), CalcRate(173), CalcRate(174), CalcRate(175),
    CalcRate(176), CalcRate(177), CalcRate(178), CalcRate(179),
    CalcRate(180), CalcRate(181), CalcRate(182), CalcRate(183),
    CalcRate(184), CalcRate(185), CalcRate(186), CalcRate(187),
    CalcRate(188), CalcRate(189), CalcRate(190), CalcRate(191),
    CalcRate(192), CalcRate(193), CalcRate(194), CalcRate(195),
    CalcRate(196), CalcRate(197), CalcRate(198), CalcRate(199),
    CalcRate(200), CalcRate(201), CalcRate(202), CalcRate(203),
    CalcRate(204), CalcRate(205), CalcRate(206), CalcRate(207),
    CalcRate(208), CalcRate(209), CalcRate(210), CalcRate(211),
    CalcRate(212), CalcRate(213), CalcRate(214), CalcRate(215),
    CalcRate(216), CalcRate(217), CalcRate(218), CalcRate(219),
    CalcRate(220), CalcRate(221), CalcRate(222), CalcRate(223),
    CalcRate(224), CalcRate(225), CalcRate(226), CalcRate(227),
    CalcRate(228), CalcRate(229), CalcRate(230), CalcRate(231),
    CalcRate(232), CalcRate(233), CalcRate(234), CalcRate(235),
    CalcRate(236), CalcRate(237), CalcRate(238), CalcRate(239),
    CalcRate(240), CalcRate(241), CalcRate(242), CalcRate(243),
    CalcRate(244), CalcRate(245), CalcRate(246), CalcRate(247),
    CalcRate(248), CalcRate(249), CalcRate(250), CalcRate(251),
    CalcRate(252), CalcRate(253), CalcRate(254), CalcRate(255),
  };
};

template<uint32_t Rate>
constexpr uint32_t CRC32Calculator<Rate>::crc32_table[]; // only change here

#endif // CRC32_CALCULATOR_H_
