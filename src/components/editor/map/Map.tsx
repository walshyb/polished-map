import Block from "./Block";
import { useAppSelector } from "../../../hooks";
import { calculateMapSizes } from "../../../utils/helper-funcs";
import "./Map.scss";

interface MapProps {
  metatiles: any;
}

export default function Map({ metatiles }: MapProps) {
  const blocks = useAppSelector((state) => state.editor.blocks);
  const file = useAppSelector((state) => state.file.activeFile);
  let cols = 0,
    rows = 0;

  if (file) {
    // @ts-ignore
    cols = calculateMapSizes(file.size).width;
    // @ts-ignore
    rows = calculateMapSizes(file.size).height;
  }

  let blockGrid = [];

  for (let i = 0; i < rows; i++) {
    const row = [];
    for (let j = 0; j < cols; j++) {
      const index = i * cols + j;
      const block = blocks[index];

      // TODO:
      // this check is temporary while i have getBlocks() call size hardcoded to 360
      // we shouldn't need this
      if (block && block.id !== null && block.id !== undefined) {
        row.push(
          <Block
            id={block.id}
            row={block.row}
            col={block.col}
            metatileImage={metatiles[block.id]}
          />,
        );
      }
    }
    blockGrid.push(
      <div key={i} className="row" style={{ display: "flex" }}>
        {row}
      </div>,
    );
  }

  return <div>{blockGrid}</div>;
}
