-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jan 27, 2025 at 10:22 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `online clothing store`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

CREATE TABLE `admin` (
  `adminid` varchar(5) NOT NULL,
  `username` varchar(50) DEFAULT NULL,
  `password` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`adminid`, `username`, `password`) VALUES
('A0001', 'Jiayee', '12345'),
('A0002', 'Kiki', '12345'),
('A0003', 'Qwerty', '123'),
('A0004', 'Ali', '12345'),
('A0005', 'Sarah', '12345'),
('A0006', 'Farah', '123');

--
-- Triggers `admin`
--
DELIMITER $$
CREATE TRIGGER `before_admin_insert` BEFORE INSERT ON `admin` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(5);

    -- Get the highest adminid
    SELECT MAX(CAST(SUBSTRING(adminid, 2) AS UNSIGNED)) INTO next_id FROM admin;
    
    -- Increment the ID and format it
    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;
    
    -- Generate the adminid in the format A0001, A0002, etc.
    SET formatted_id = CONCAT('A', LPAD(next_id, 4, '0'));
    
    -- Set the new adminid for the new record
    SET NEW.adminid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `cart`
--

CREATE TABLE `cart` (
  `cartid` varchar(5) NOT NULL,
  `customerid` varchar(5) NOT NULL,
  `sizeid` varchar(4) NOT NULL,
  `quantity` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Triggers `cart`
--
DELIMITER $$
CREATE TRIGGER `before_cart_insert` BEFORE INSERT ON `cart` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(5);

    -- Safely get the highest numeric part of cartid
    SELECT MAX(CAST(SUBSTRING(cartid, 3) AS UNSIGNED)) INTO next_id FROM cart ;

    -- Increment the ID and handle the case where the table is empty
    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;

    -- Format the new ID as 'CT001', 'CT002', etc.
    SET formatted_id = CONCAT('CT', LPAD(next_id, 3, '0'));

    -- Set the new cartid
    SET NEW.cartid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `category`
--

CREATE TABLE `category` (
  `categoryid` varchar(4) NOT NULL,
  `category_name` varchar(100) NOT NULL,
  `description` text NOT NULL,
  `gender` enum('Men','Women') NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `category`
--

INSERT INTO `category` (`categoryid`, `category_name`, `description`, `gender`) VALUES
('CA01', 'Casual Wear', 'Casual attire is generally less formal than business attire. This type of clothes is stylish outfits for casual occasions and much more comfortable.', 'Men'),
('CA02', 'Casual Wear', 'Casual attire is generally less formal than business attire. This type of clothes is stylish outfits for casual occasions and much more comfortable.', 'Women'),
('CA03', 'Everyday Attire', 'A combination of comfort and individual flair, everyday attire is the foundation of daily fashion. Easy morning looks come from a basic dress or a pair of trusty denim pants worn with a favorite t-shirt.', 'Men'),
('CA04', 'Everyday Attire', 'A combination of comfort and individual flair, everyday attire is the foundation of daily fashion. Easy morning looks come from a basic dress or a pair of trusty denim pants worn with a favorite t-shirt.', 'Women'),
('CA05', 'Formal Clothing', 'Formal wear is this type of clothing that is designed for working people or office wear. These clothes are tailored, fit well, and show professionalism and respect.', 'Men'),
('CA06', 'Formal Clothing', 'Formal wear is this type of clothing that is designed for working people or office wear. These clothes are tailored, fit well, and show professionalism and respect.', 'Women'),
('CA07', 'Sportswear', 'Activewear designed for sports and physical activities. Manufactured of flexible and versatile fabric, this clothing offers excellent comfort.', 'Men'),
('CA08', 'Sportswear', 'Activewear designed for sports and physical activities. Manufactured of flexible and versatile fabric, this clothing offers excellent comfort.', 'Women'),
('CA09', 'Sleepwear', 'Sleepwear is a particular type of clothing that is generally manufactured to wear during your bedtime is bedtime clothing. It’s all about feeling relaxed and comfortable, aiding you to unwind after a long day.', 'Men'),
('CA10', 'Sleepwear', 'Sleepwear is a particular type of clothing that is generally manufactured to wear during your bedtime is bedtime clothing. It’s all about feeling relaxed and comfortable, aiding you to unwind after a long day.', 'Women');

--
-- Triggers `category`
--
DELIMITER $$
CREATE TRIGGER `before_category_insert` BEFORE INSERT ON `category` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(4);

    -- Safely get the highest numeric part of categoryid
    SELECT MAX(CAST(SUBSTRING(categoryid, 3) AS UNSIGNED)) INTO next_id FROM category;

    -- Increment the ID and handle the case where the table is empty
    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;

    -- Format the new ID as 'CA01', 'CA02', etc.
    SET formatted_id = CONCAT('CA', LPAD(next_id, 2, '0'));

    -- Set the new categoryid
    SET NEW.categoryid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE `customer` (
  `customerid` varchar(5) NOT NULL,
  `username` varchar(50) DEFAULT NULL,
  `password` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`customerid`, `username`, `password`) VALUES
('C0001', 'Jiayee', '123'),
('C0002', 'Lili', '123'),
('C0003', 'Kathy', '123'),
('C0004', 'Kathy', '123'),
('C0005', 'Haha', '123'),
('C0006', 'Ali', '123'),
('C0007', 'John', '123'),
('C0008', 'Jane', '123'),
('C0009', 'Emily', '123'),
('C0010', 'Chris', '123'),
('C0011', 'Anna', '123'),
('C0012', 'Mike', '123'),
('C0013', 'Sarah', '123'),
('C0014', 'John', '123'),
('C0015', 'Alice', '123');

--
-- Triggers `customer`
--
DELIMITER $$
CREATE TRIGGER `before_customer_insert` BEFORE INSERT ON `customer` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(5);

    -- Get the highest adminid
    SELECT MAX(CAST(SUBSTRING(customerid, 2) AS UNSIGNED)) INTO next_id FROM customer;
    
    -- Increment the ID and format it
    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;
    
    -- Generate the adminid in the format C0001, C0002, etc.
    SET formatted_id = CONCAT('C', LPAD(next_id, 4, '0'));
    
    -- Set the new adminid for the new record
    SET NEW.customerid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

CREATE TABLE `orders` (
  `orderid` varchar(5) NOT NULL,
  `customerid` varchar(5) NOT NULL,
  `order_date` datetime NOT NULL,
  `total_amount` decimal(10,2) NOT NULL,
  `status` varchar(20) DEFAULT 'Pending'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `orders`
--

INSERT INTO `orders` (`orderid`, `customerid`, `order_date`, `total_amount`, `status`) VALUES
('O0001', 'C0002', '2025-01-23 18:41:47', 79.98, 'Delivered'),
('O0002', 'C0001', '2025-01-24 00:58:26', 323.96, 'Paid'),
('O0003', 'C0002', '2025-01-24 01:08:02', 43.98, 'Paid'),
('O0004', 'C0001', '2025-01-24 03:34:08', 188.96, 'Paid'),
('O0005', 'C0001', '2025-01-24 03:41:18', 316.76, 'Paid'),
('O0006', 'C0001', '2025-01-24 03:44:57', 180.46, 'Pending'),
('O0007', 'C0001', '2025-01-24 03:50:38', 123.46, 'Pending'),
('O0008', 'C0001', '2025-01-24 03:56:20', 180.46, 'Pending'),
('O0009', 'C0001', '2025-01-24 03:58:16', 69.97, 'Pending'),
('O0010', 'C0001', '2025-01-24 03:59:31', 161.46, 'Paid'),
('O0011', 'C0001', '2025-01-24 04:07:04', 180.46, 'Paid'),
('O0012', 'C0001', '2025-01-24 04:10:59', 269.96, 'Paid'),
('O0013', 'C0001', '2025-01-24 07:06:24', 296.97, 'Paid'),
('O0014', 'C0002', '2025-01-24 07:17:21', 242.95, 'Paid'),
('O0015', 'C0002', '2025-01-24 07:31:12', 219.97, 'Paid'),
('O0016', 'C0001', '2024-01-01 00:00:00', 150.00, 'Paid'),
('O0017', 'C0002', '2024-01-05 00:00:00', 200.00, 'Paid'),
('O0018', 'C0003', '2024-01-10 00:00:00', 250.00, 'Paid'),
('O0019', 'C0004', '2024-02-01 00:00:00', 180.00, 'Paid'),
('O0020', 'C0005', '2024-02-05 00:00:00', 220.00, 'Paid'),
('O0021', 'C0001', '2024-02-10 00:00:00', 270.00, 'Paid'),
('O0022', 'C0002', '2024-03-02 00:00:00', 320.00, 'Paid'),
('O0023', 'C0003', '2024-03-07 00:00:00', 180.00, 'Paid'),
('O0024', 'C0004', '2024-04-01 00:00:00', 450.00, 'Paid'),
('O0025', 'C0005', '2024-04-15 00:00:00', 500.00, 'Paid'),
('O0026', 'C0001', '2024-05-03 00:00:00', 350.00, 'Paid'),
('O0027', 'C0002', '2024-05-10 00:00:00', 290.00, 'Paid'),
('O0028', 'C0003', '2024-06-01 00:00:00', 230.00, 'Paid'),
('O0029', 'C0004', '2024-06-15 00:00:00', 400.00, 'Paid'),
('O0030', 'C0005', '2024-07-05 00:00:00', 330.00, 'Paid'),
('O0031', 'C0001', '2024-07-10 00:00:00', 200.00, 'Paid'),
('O0032', 'C0002', '2024-08-01 00:00:00', 210.00, 'Paid'),
('O0033', 'C0003', '2024-08-15 00:00:00', 370.00, 'Paid'),
('O0034', 'C0004', '2024-09-02 00:00:00', 420.00, 'Paid'),
('O0035', 'C0005', '2024-09-10 00:00:00', 250.00, 'Paid'),
('O0036', 'C0001', '2024-10-05 00:00:00', 300.00, 'Paid'),
('O0037', 'C0002', '2024-10-12 00:00:00', 150.00, 'Paid'),
('O0038', 'C0003', '2024-11-01 00:00:00', 500.00, 'Paid'),
('O0039', 'C0004', '2024-11-15 00:00:00', 550.00, 'Paid'),
('O0040', 'C0005', '2024-12-01 00:00:00', 280.00, 'Paid'),
('O0041', 'C0001', '2024-12-05 00:00:00', 450.00, 'Paid'),
('O0042', 'C0001', '2025-01-24 09:14:12', 259.98, 'Paid'),
('O0043', 'C0006', '2025-01-24 09:47:53', 259.96, 'Shipped'),
('O0044', 'C0001', '2024-01-15 10:00:00', 179.97, 'Paid'),
('O0045', 'C0002', '2024-02-16 11:00:00', 119.97, 'Paid'),
('O0046', 'C0003', '2024-03-17 12:00:00', 199.98, 'Paid'),
('O0047', 'C0004', '2024-04-18 13:00:00', 129.97, 'Paid'),
('O0048', 'C0005', '2024-05-19 14:00:00', 259.96, 'Paid'),
('O0049', 'C0006', '2024-06-20 15:00:00', 149.97, 'Paid'),
('O0050', 'C0001', '2024-07-21 16:00:00', 179.97, 'Paid'),
('O0051', 'C0002', '2024-08-22 17:00:00', 169.96, 'Paid'),
('O0052', 'C0003', '2024-09-23 18:00:00', 199.95, 'Paid'),
('O0053', 'C0004', '2024-10-24 19:00:00', 219.98, 'Paid'),
('O0054', 'C0005', '2024-11-25 20:00:00', 189.97, 'Paid'),
('O0055', 'C0006', '2024-12-26 21:00:00', 249.95, 'Paid'),
('O0056', 'C0015', '2025-01-24 15:11:29', 199.95, 'Delivered'),
('O0057', 'C0001', '2025-01-25 17:55:16', 199.97, 'Paid');

--
-- Triggers `orders`
--
DELIMITER $$
CREATE TRIGGER `before_orders_insert` BEFORE INSERT ON `orders` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(5);

    SELECT MAX(CAST(SUBSTRING(orderid, 2) AS UNSIGNED)) INTO next_id FROM orders;

    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;

    SET formatted_id = CONCAT('O', LPAD(next_id, 4, '0'));
    SET NEW.orderid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `order_items`
--

CREATE TABLE `order_items` (
  `orderitemid` varchar(6) NOT NULL,
  `orderid` varchar(5) NOT NULL,
  `sizeid` varchar(4) NOT NULL,
  `quantity` int(11) NOT NULL,
  `unit_price` decimal(10,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `order_items`
--

INSERT INTO `order_items` (`orderitemid`, `orderid`, `sizeid`, `quantity`, `unit_price`) VALUES
('OI0001', 'O0001', 'S010', 1, 39.99),
('OI0002', 'O0001', 'S011', 1, 39.99),
('OI0003', 'O0002', 'S001', 1, 21.99),
('OI0004', 'O0002', 'S002', 1, 21.99),
('OI0005', 'O0002', 'S016', 2, 139.99),
('OI0006', 'O0003', 'S001', 2, 21.99),
('OI0007', 'O0004', 'S012', 1, 39.99),
('OI0008', 'O0004', 'S022', 3, 59.99),
('OI0009', 'O0005', 'S002', 1, 21.99),
('OI0010', 'O0005', 'S011', 2, 39.99),
('OI0011', 'O0005', 'S014', 2, 129.99),
('OI0012', 'O0006', 'S011', 2, 39.99),
('OI0013', 'O0006', 'S019', 2, 59.99),
('OI0014', 'O0007', 'S012', 2, 39.99),
('OI0015', 'O0007', 'S026', 2, 29.99),
('OI0016', 'O0008', 'S012', 2, 39.99),
('OI0017', 'O0008', 'S021', 2, 59.99),
('OI0018', 'O0009', 'S004', 2, 19.99),
('OI0019', 'O0009', 'S010', 1, 39.99),
('OI0020', 'O0010', 'S019', 2, 59.99),
('OI0021', 'O0010', 'S030', 2, 29.99),
('OI0022', 'O0011', 'S011', 2, 39.99),
('OI0023', 'O0011', 'S019', 2, 59.99),
('OI0024', 'O0012', 'S022', 3, 59.99),
('OI0025', 'O0012', 'S014', 1, 129.99),
('OI0026', 'O0013', 'S017', 2, 139.99),
('OI0027', 'O0013', 'S022', 1, 59.99),
('OI0028', 'O0014', 'S021', 2, 59.99),
('OI0029', 'O0014', 'S019', 1, 59.99),
('OI0030', 'O0014', 'S007', 1, 39.99),
('OI0031', 'O0014', 'S030', 2, 29.99),
('OI0032', 'O0015', 'S030', 1, 29.99),
('OI0033', 'O0015', 'S015', 1, 129.99),
('OI0034', 'O0015', 'S022', 1, 59.99),
('OI0035', 'O0042', 'S014', 2, 129.99),
('OI0036', 'O0043', 'S007', 1, 39.99),
('OI0037', 'O0043', 'S017', 1, 139.99),
('OI0038', 'O0043', 'S007', 2, 39.99),
('OI0039', 'O0044', 'S031', 3, 29.99),
('OI0040', 'O0044', 'S033', 2, 9.99),
('OI0041', 'O0045', 'S032', 2, 49.99),
('OI0042', 'O0045', 'S034', 3, 9.99),
('OI0043', 'O0046', 'S031', 4, 29.99),
('OI0044', 'O0046', 'S033', 2, 9.99),
('OI0045', 'O0047', 'S032', 3, 49.99),
('OI0046', 'O0047', 'S034', 2, 9.99),
('OI0047', 'O0048', 'S031', 4, 29.99),
('OI0048', 'O0048', 'S033', 3, 9.99),
('OI0049', 'O0049', 'S032', 2, 49.99),
('OI0050', 'O0049', 'S034', 2, 9.99),
('OI0051', 'O0050', 'S031', 2, 29.99),
('OI0052', 'O0050', 'S033', 3, 9.99),
('OI0053', 'O0051', 'S032', 3, 49.99),
('OI0054', 'O0051', 'S034', 2, 9.99),
('OI0055', 'O0052', 'S031', 2, 29.99),
('OI0056', 'O0052', 'S033', 3, 9.99),
('OI0057', 'O0053', 'S032', 3, 49.99),
('OI0058', 'O0053', 'S034', 2, 9.99),
('OI0059', 'O0056', 'S010', 5, 39.99),
('OI0060', 'O0057', 'S012', 1, 39.99),
('OI0061', 'O0057', 'S004', 1, 19.99),
('OI0062', 'O0057', 'S017', 1, 139.99);

--
-- Triggers `order_items`
--
DELIMITER $$
CREATE TRIGGER `before_order_items_insert` BEFORE INSERT ON `order_items` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(6);

    -- Generate sequential orderitemid
    SELECT MAX(CAST(SUBSTRING(orderitemid, 3) AS UNSIGNED)) INTO next_id FROM order_items;

    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;

    SET formatted_id = CONCAT('OI', LPAD(next_id, 4, '0'));
    SET NEW.orderitemid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `payment`
--

CREATE TABLE `payment` (
  `paymentid` varchar(6) NOT NULL,
  `orderid` varchar(5) NOT NULL,
  `customerid` varchar(5) NOT NULL,
  `payment_method` varchar(20) NOT NULL,
  `payment_date` datetime NOT NULL,
  `amount` decimal(10,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `payment`
--

INSERT INTO `payment` (`paymentid`, `orderid`, `customerid`, `payment_method`, `payment_date`, `amount`) VALUES
('PM0001', 'O0002', 'C0001', 'cash', '2025-01-24 00:59:08', 323.96),
('PM0002', 'O0003', 'C0002', 'Cash', '2025-01-24 01:08:18', 43.98),
('PM0003', 'O0004', 'C0001', 'COD', '2025-01-24 03:34:21', 188.96),
('PM0004', 'O0005', 'C0001', 'COD', '2025-01-24 03:41:23', 316.76),
('PM0005', 'O0010', 'C0001', 'cod', '2025-01-24 03:59:38', 161.46),
('PM0006', 'O0011', 'C0001', 'cod', '2025-01-24 04:07:08', 180.46),
('PM0007', 'O0012', 'C0001', 'COD', '2025-01-24 04:11:02', 269.96),
('PM0008', 'O0013', 'C0001', 'cod', '2025-01-24 07:06:28', 296.97),
('PM0009', 'O0014', 'C0002', 'Credit Card', '2025-01-24 07:17:21', 227.95),
('PM0010', 'O0014', 'C0002', 'cod', '2025-01-24 07:17:25', 227.95),
('PM0011', 'O0015', 'C0002', 'Credit Card', '2025-01-24 07:31:12', 172.97),
('PM0012', 'O0015', 'C0002', 'PAYPAL', '2025-01-24 07:31:17', 172.97),
('PM0013', 'O0016', 'C0001', 'Credit Card', '2024-01-02 00:00:00', 150.00),
('PM0014', 'O0017', 'C0002', 'Credit Card', '2024-01-06 00:00:00', 200.00),
('PM0015', 'O0018', 'C0003', 'Credit Card', '2024-01-11 00:00:00', 250.00),
('PM0016', 'O0019', 'C0004', 'Credit Card', '2024-02-02 00:00:00', 180.00),
('PM0017', 'O0020', 'C0005', 'Credit Card', '2024-02-06 00:00:00', 220.00),
('PM0018', 'O0021', 'C0001', 'Credit Card', '2024-02-11 00:00:00', 270.00),
('PM0019', 'O0022', 'C0002', 'Credit Card', '2024-03-03 00:00:00', 320.00),
('PM0020', 'O0023', 'C0003', 'Credit Card', '2024-03-08 00:00:00', 180.00),
('PM0021', 'O0024', 'C0004', 'Credit Card', '2024-04-02 00:00:00', 450.00),
('PM0022', 'O0025', 'C0005', 'Credit Card', '2024-04-16 00:00:00', 500.00),
('PM0023', 'O0026', 'C0001', 'Credit Card', '2024-05-04 00:00:00', 350.00),
('PM0024', 'O0027', 'C0002', 'Credit Card', '2024-05-11 00:00:00', 290.00),
('PM0025', 'O0028', 'C0003', 'Credit Card', '2024-06-02 00:00:00', 230.00),
('PM0026', 'O0029', 'C0004', 'Credit Card', '2024-06-16 00:00:00', 400.00),
('PM0027', 'O0030', 'C0005', 'Credit Card', '2024-07-06 00:00:00', 330.00),
('PM0028', 'O0031', 'C0001', 'Credit Card', '2024-07-11 00:00:00', 200.00),
('PM0029', 'O0032', 'C0002', 'Credit Card', '2024-08-02 00:00:00', 210.00),
('PM0030', 'O0033', 'C0003', 'Credit Card', '2024-08-16 00:00:00', 370.00),
('PM0031', 'O0034', 'C0004', 'Credit Card', '2024-09-03 00:00:00', 420.00),
('PM0032', 'O0035', 'C0005', 'Credit Card', '2024-09-11 00:00:00', 250.00),
('PM0033', 'O0036', 'C0001', 'Credit Card', '2024-10-06 00:00:00', 300.00),
('PM0034', 'O0037', 'C0002', 'Credit Card', '2024-10-13 00:00:00', 150.00),
('PM0035', 'O0038', 'C0003', 'Credit Card', '2024-11-02 00:00:00', 500.00),
('PM0036', 'O0039', 'C0004', 'Credit Card', '2024-11-16 00:00:00', 550.00),
('PM0037', 'O0040', 'C0005', 'Credit Card', '2024-12-02 00:00:00', 280.00),
('PM0038', 'O0041', 'C0001', 'Credit Card', '2024-12-06 00:00:00', 450.00),
('PM0039', 'O0042', 'C0001', 'Credit Card', '2025-01-24 09:14:12', 223.98),
('PM0040', 'O0042', 'C0001', 'cod', '2025-01-24 09:14:19', 223.98),
('PM0041', 'O0043', 'C0006', 'Credit Card', '2025-01-24 09:47:53', 208.96),
('PM0042', 'O0043', 'C0006', 'cod', '2025-01-24 09:48:07', 208.96),
('PM0043', 'O0044', 'C0001', 'Credit Card', '2024-01-15 10:30:00', 179.97),
('PM0044', 'O0045', 'C0002', 'PayPal', '2024-02-16 11:30:00', 119.97),
('PM0045', 'O0046', 'C0003', 'Credit Card', '2024-03-17 12:30:00', 199.98),
('PM0046', 'O0047', 'C0004', 'Debit Card', '2024-04-18 13:30:00', 129.97),
('PM0047', 'O0048', 'C0005', 'Credit Card', '2024-05-19 14:30:00', 259.96),
('PM0048', 'O0049', 'C0006', 'PayPal', '2024-06-20 15:30:00', 149.97),
('PM0049', 'O0050', 'C0001', 'Debit Card', '2024-07-21 16:30:00', 179.97),
('PM0050', 'O0051', 'C0002', 'Credit Card', '2024-08-22 17:30:00', 169.96),
('PM0051', 'O0052', 'C0003', 'PayPal', '2024-09-23 18:30:00', 199.95),
('PM0052', 'O0053', 'C0004', 'Credit Card', '2024-10-24 19:30:00', 219.98),
('PM0053', 'O0054', 'C0005', 'Debit Card', '2024-11-25 20:30:00', 189.97),
('PM0054', 'O0055', 'C0006', 'PayPal', '2024-12-26 21:30:00', 249.95),
('PM0055', 'O0056', 'C0015', 'Credit Card', '2025-01-24 15:11:29', 164.95),
('PM0056', 'O0056', 'C0015', 'cod', '2025-01-24 15:12:10', 164.95),
('PM0057', 'O0057', 'C0001', 'Credit Card', '2025-01-25 17:55:16', 179.97),
('PM0058', 'O0057', 'C0001', 'cod', '2025-01-25 17:55:45', 179.97);

--
-- Triggers `payment`
--
DELIMITER $$
CREATE TRIGGER `before_payment_insert` BEFORE INSERT ON `payment` FOR EACH ROW BEGIN
    DECLARE next_id INT DEFAULT 0;  -- Initialize the next_id variable
    DECLARE formatted_id VARCHAR(6); -- To hold the formatted payment ID

    -- Fetch the highest numeric value from the paymentid
    SELECT MAX(CAST(SUBSTRING(paymentid, 3) AS UNSIGNED)) INTO next_id
    FROM payment;

    -- Increment the ID or initialize it to 1 if the table is empty
    SET next_id = IFNULL(next_id, 0) + 1;

    -- Generate the new paymentid in the format PM0001, PM0002, etc.
    SET formatted_id = CONCAT('PM', LPAD(next_id, 4, '0'));

    -- Assign the generated ID to the NEW row
    SET NEW.paymentid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `product`
--

CREATE TABLE `product` (
  `productid` varchar(5) NOT NULL,
  `product_name` varchar(100) NOT NULL,
  `description` text NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `categoryid` varchar(4) NOT NULL,
  `cost_price` decimal(10,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `product`
--

INSERT INTO `product` (`productid`, `product_name`, `description`, `price`, `categoryid`, `cost_price`) VALUES
('P0001', 'Men\'s Casual T-shirt', 'A comfortable cotton T-shirt for casual wear', 21.99, 'CA01', 15.99),
('P0002', 'Women\'s Casual T-shirt', 'A comfortable cotton T-shirt for casual wear', 19.99, 'CA02', 14.99),
('P0003', 'Men\'s Jeans', 'Stylish denim jeans for everyday use', 39.99, 'CA03', 34.99),
('P0004', 'Women\'s Jeans', 'Stylish denim jeans for everyday use', 39.99, 'CA04', 30.99),
('P0005', 'Men\'s Suit', 'Formal suit for business meetings', 129.99, 'CA05', 110.99),
('P0006', 'Women\'s Suit', 'Formal suit for business meetings', 139.99, 'CA06', 120.99),
('P0007', 'Men\'s Running Shoes', 'Comfortable running shoes', 59.99, 'CA07', 49.99),
('P0008', 'Women\'s Running Shoes', 'Comfortable running shoes', 59.99, 'CA08', 50.99),
('P0009', 'Men\'s Pajama Set', 'Comfortable sleepwear for men', 29.99, 'CA09', 20.99),
('P0010', 'Women\'s Pajama Set', 'Comfortable sleepwear for women', 29.99, 'CA10', 20.99),
('P0012', 'Black T-Shirt', 'Soft and Comfortable', 19.99, 'CA01', 10.99),
('P0013', 'q', 'q', 12.00, 'CA10', 10.00),
('P0014', 'Shoes', 'Comfortable leather shoes', 89.99, 'CA01', 60.99),
('P0015', 'Socks', 'Soft cotton socks', 9.99, 'CA02', 5.99),
('P0016', 'Shoes', 'Comfortable leather shoes', 89.99, 'CA01', 60.99),
('P0017', 'Socks', 'Soft cotton socks', 9.99, 'CA02', 4.99),
('P0018', 'Shoes', 'Comfortable leather shoes', 89.99, 'CA01', 69.99),
('P0019', 'Socks', 'Soft cotton socks', 9.99, 'CA02', 4.99),
('P0020', 'Shoes', 'Comfortable leather shoes', 89.99, 'CA01', 59.99),
('P0021', 'Socks', 'Soft cotton socks', 9.99, 'CA02', 5.99),
('P0022', 'Shoes', 'Comfortable leather shoes', 89.99, 'CA01', 79.99),
('P0023', 'Socks', 'Soft cotton socks', 9.99, 'CA02', 3.99),
('P0024', 'Shoes', 'Comfortable leather shoes', 89.99, 'CA01', 79.99),
('P0025', 'Socks', 'Soft cotton socks', 9.99, 'CA02', 4.99);

--
-- Triggers `product`
--
DELIMITER $$
CREATE TRIGGER `before_product_insert` BEFORE INSERT ON `product` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(5);

    -- Safely get the highest numeric part of productid
    SELECT MAX(CAST(SUBSTRING(productid, 2) AS UNSIGNED)) INTO next_id FROM product;

    -- Increment the ID and handle the case where the table is empty
    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;

    -- Format the new ID as 'P0001', 'P0002', etc.
    SET formatted_id = CONCAT('P', LPAD(next_id, 4, '0'));

    -- Set the new productid
    SET NEW.productid = formatted_id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `product_size`
--

CREATE TABLE `product_size` (
  `sizeid` varchar(4) NOT NULL,
  `size` varchar(20) NOT NULL,
  `productid` varchar(5) NOT NULL,
  `stock` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Dumping data for table `product_size`
--

INSERT INTO `product_size` (`sizeid`, `size`, `productid`, `stock`) VALUES
('S001', 'Small', 'P0001', 47),
('S002', 'Medium', 'P0001', 18),
('S003', 'Large', 'P0001', 20),
('S004', 'Small', 'P0002', 53),
('S005', 'Medium', 'P0002', 90),
('S006', 'Large', 'P0002', 40),
('S007', '32', 'P0003', 54),
('S008', '34', 'P0003', 70),
('S009', '36', 'P0003', 30),
('S010', '28', 'P0004', 33),
('S011', '30', 'P0004', 56),
('S012', '32', 'P0004', 53),
('S013', 'M', 'P0005', 40),
('S014', 'L', 'P0005', 15),
('S015', 'XL', 'P0005', 15),
('S016', 'M', 'P0006', 48),
('S017', 'L', 'P0006', 53),
('S018', 'XL', 'P0006', 39),
('S019', '7', 'P0007', 56),
('S020', '8', 'P0007', 118),
('S021', '9', 'P0007', 56),
('S022', '6', 'P0008', 15),
('S023', '7', 'P0008', 90),
('S024', '8', 'P0008', 100),
('S025', 'M', 'P0009', 50),
('S026', 'L', 'P0009', 58),
('S027', 'XL', 'P0009', 40),
('S028', 'S', 'P0010', 70),
('S029', 'M', 'P0010', 80),
('S030', 'L', 'P0010', 15),
('S031', 's', 'P0013', 30),
('S032', 'Small', 'P0014', 40),
('S033', 'Medium', 'P0014', 60),
('S034', 'Large', 'P0015', 30),
('S035', 'X-Large', 'P0015', 100),
('S036', 'Small', 'P0014', 40),
('S037', 'Medium', 'P0014', 60),
('S038', 'Large', 'P0015', 30),
('S039', 'X-Large', 'P0015', 100),
('S040', 'Small', 'P0014', 40),
('S041', 'Medium', 'P0014', 60),
('S042', 'Large', 'P0015', 30),
('S043', 'X-Large', 'P0015', 100),
('S044', 'Small', 'P0014', 40),
('S045', 'Medium', 'P0014', 60),
('S046', 'Large', 'P0015', 30),
('S047', 'X-Large', 'P0015', 100),
('S048', 'Small', 'P0014', 40),
('S049', 'Medium', 'P0014', 60),
('S050', 'Large', 'P0015', 30),
('S051', 'X-Large', 'P0015', 100),
('S052', 'Small', 'P0014', 40),
('S053', 'Medium', 'P0014', 60),
('S054', 'Large', 'P0015', 30),
('S055', 'X-Large', 'P0015', 100);

--
-- Triggers `product_size`
--
DELIMITER $$
CREATE TRIGGER `before_product_size_insert` BEFORE INSERT ON `product_size` FOR EACH ROW BEGIN
    DECLARE next_id INT;
    DECLARE formatted_id VARCHAR(4);

    -- Get the highest numeric part of sizeid from product_size table
    SELECT MAX(CAST(SUBSTRING(sizeid, 2) AS UNSIGNED)) INTO next_id FROM product_size;

    -- Increment the ID and handle the case where the table is empty
    IF next_id IS NULL THEN
        SET next_id = 1;
    ELSE
        SET next_id = next_id + 1;
    END IF;

    -- Format the new ID as 'S001', 'S002', etc.
    SET formatted_id = CONCAT('S', LPAD(next_id, 3, '0'));

    -- Set the new sizeid
    SET NEW.sizeid = formatted_id;
END
$$
DELIMITER ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `admin`
--
ALTER TABLE `admin`
  ADD PRIMARY KEY (`adminid`);

--
-- Indexes for table `cart`
--
ALTER TABLE `cart`
  ADD PRIMARY KEY (`cartid`),
  ADD KEY `sizeid` (`sizeid`),
  ADD KEY `customerid` (`customerid`);

--
-- Indexes for table `category`
--
ALTER TABLE `category`
  ADD PRIMARY KEY (`categoryid`);

--
-- Indexes for table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`customerid`);

--
-- Indexes for table `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`orderid`),
  ADD KEY `customerid` (`customerid`);

--
-- Indexes for table `order_items`
--
ALTER TABLE `order_items`
  ADD PRIMARY KEY (`orderitemid`),
  ADD KEY `orderid` (`orderid`),
  ADD KEY `sizeid` (`sizeid`);

--
-- Indexes for table `payment`
--
ALTER TABLE `payment`
  ADD PRIMARY KEY (`paymentid`),
  ADD KEY `orderid` (`orderid`),
  ADD KEY `customerid` (`customerid`);

--
-- Indexes for table `product`
--
ALTER TABLE `product`
  ADD PRIMARY KEY (`productid`),
  ADD KEY `categoryid` (`categoryid`);

--
-- Indexes for table `product_size`
--
ALTER TABLE `product_size`
  ADD PRIMARY KEY (`sizeid`),
  ADD KEY `productid` (`productid`);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `cart`
--
ALTER TABLE `cart`
  ADD CONSTRAINT `cart_ibfk_1` FOREIGN KEY (`sizeid`) REFERENCES `product_size` (`sizeid`),
  ADD CONSTRAINT `cart_ibfk_2` FOREIGN KEY (`customerid`) REFERENCES `customer` (`customerid`);

--
-- Constraints for table `orders`
--
ALTER TABLE `orders`
  ADD CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`customerid`) REFERENCES `customer` (`customerid`);

--
-- Constraints for table `order_items`
--
ALTER TABLE `order_items`
  ADD CONSTRAINT `order_items_ibfk_1` FOREIGN KEY (`orderid`) REFERENCES `orders` (`orderid`),
  ADD CONSTRAINT `order_items_ibfk_2` FOREIGN KEY (`sizeid`) REFERENCES `product_size` (`sizeid`);

--
-- Constraints for table `payment`
--
ALTER TABLE `payment`
  ADD CONSTRAINT `payment_ibfk_1` FOREIGN KEY (`orderid`) REFERENCES `orders` (`orderid`),
  ADD CONSTRAINT `payment_ibfk_2` FOREIGN KEY (`customerid`) REFERENCES `customer` (`customerid`);

--
-- Constraints for table `product`
--
ALTER TABLE `product`
  ADD CONSTRAINT `product_ibfk_1` FOREIGN KEY (`categoryid`) REFERENCES `category` (`categoryid`);

--
-- Constraints for table `product_size`
--
ALTER TABLE `product_size`
  ADD CONSTRAINT `product_size_ibfk_1` FOREIGN KEY (`productid`) REFERENCES `product` (`productid`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
