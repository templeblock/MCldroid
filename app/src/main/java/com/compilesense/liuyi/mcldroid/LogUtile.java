package com.compilesense.liuyi.mcldroid;

import java.io.FileWriter;

/**
 * Created by shenjingyuan002 on 2017/3/31.
 */

public class LogUtile {

    static final String logPath = "/storage/emulated/0/MCLdroidTestLog/";

    static void saveLogData(String fileName, float[][][][] data, int[] indexs) throws Exception {

        FileWriter fw = new FileWriter(logPath + fileName);
        StringBuilder sb = new StringBuilder();
        sb.append("shape: ")
                .append(indexs[0]).append("*")
                .append(indexs[1]).append("*")
                .append(indexs[2]).append("*")
                .append(indexs[3]).append("\n");
        fw.write(sb.toString());
        sb.delete(0,sb.length());
        int nSize = indexs[0];
        int cSize = indexs[1];
        int hSize = indexs[2];
        int wSize = indexs[3];
        for (int indexN = 0; indexN < nSize; ++indexN){
            for (int indexC = 0; indexC < cSize; ++indexC){
                for (int indexH = 0; indexH < hSize; ++indexH){
                    sb.append("[")
                            .append(indexN).append("][")
                            .append(indexC).append("][")
                            .append(indexH).append("][")
                            .append("0~").append(wSize - 1).append("] \r");
                    fw.write(sb.toString());
                    sb.delete(0,sb.length());

                    for (int indexW = 0; indexW < wSize; ++indexW){
                        sb.append(data[indexN][indexC][indexH][indexW]).append("\t");
                    }
                    sb.append("\r");
                    fw.write(sb.toString());
                    sb.delete(0,sb.length());
                }
            }
        }
        fw.flush();
        fw.close();
    }

    static void saveLogData(String fileName, float[][] data, int[] indexs) throws Exception {
        FileWriter fw = new FileWriter(logPath + fileName);
        StringBuilder sb = new StringBuilder();
        sb.append("shape: ")
                .append(indexs[0]).append("*")
                .append(indexs[1]).append("\n");
        fw.write(sb.toString());
        sb.delete(0,sb.length());
        int nSize = indexs[0];
        int cSize = indexs[1];
        for (int indexN = 0; indexN < nSize; ++indexN){
            sb.append("[")
                    .append(indexN).append("][")
                    .append("0~").append(cSize).append("][")
                    .append(0).append("][")
                    .append("0").append("] \r");
            fw.write(sb.toString());
            sb.delete(0,sb.length());

            for (int indexC = 0; indexC < cSize; ++indexC){
                sb.append(data[indexN][indexC]).append("\t");
            }
            sb.append("\r");
            fw.write(sb.toString());
            sb.delete(0,sb.length());
        }
        fw.flush();
        fw.close();
    }
}
