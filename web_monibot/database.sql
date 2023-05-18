SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


CREATE TABLE `data_monibot` (
  `id` int NOT NULL,
  `data_temp` float NOT NULL,
  `data_humd` float NOT NULL,
  `data_ppmch4` float NOT NULL,
  `data_ppmco` float NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

--
ALTER TABLE `data_monibot`
  ADD PRIMARY KEY (`id`);


ALTER TABLE `data_monibot`
  MODIFY `id` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

